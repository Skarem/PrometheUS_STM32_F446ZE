#include "cppMain.hpp"

extern "C" {
#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
}

#include "PrometheUS.hpp"

// ========== Flags ==========
SystemFlags systemFlags;

// ========== Create data samplers (lowest-level globals) ==========
ClutchCurrentSampler clutchCurrentSampler1;
ClutchCurrentSampler clutchCurrentSampler2;
ClutchCurrentSampler clutchCurrentSampler3;
MotorVelocitySampler motorVelocitySampler;

int cppMain()
{
  PrometheUS_Gripper gripper(systemFlags, clutchCurrentSampler1, clutchCurrentSampler2, clutchCurrentSampler3, motorVelocitySampler);
  gripper.init();

  while (true)
  {
    // gripper.execute();
  }
}

extern "C" void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      // Flag to update PWM duty cycles
      SystemFlags::instance->updatePwmDutyCycles.store(true, std::memory_order_relaxed);

      // Read motor speed
      SystemFlags::instance->lastActiveADC.store(SystemFlags::ADCSource::MOTOR, std::memory_order_relaxed);
      MotorVelocitySampler::instance->startSamplingRawAdcValue();
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      // Start control cycle
      SystemFlags::instance->startControlCycle.store(true, std::memory_order_relaxed);

      // Read clutch current #1
      SystemFlags::instance->lastActiveADC.store(SystemFlags::ADCSource::CLUTCH_1, std::memory_order_relaxed);
      ClutchCurrentSampler::instances[FINGER_1_INDEX]->startSamplingRawAdcValue();
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      // Read clutch current #2
      SystemFlags::instance->lastActiveADC.store(SystemFlags::ADCSource::CLUTCH_2, std::memory_order_relaxed);
      ClutchCurrentSampler::instances[FINGER_2_INDEX]->startSamplingRawAdcValue();
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      // Read clutch current #3
      SystemFlags::instance->lastActiveADC.store(SystemFlags::ADCSource::CLUTCH_3, std::memory_order_relaxed);
      ClutchCurrentSampler::instances[FINGER_3_INDEX]->startSamplingRawAdcValue();
    }
  }
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
  {
    // Done reading potentiometer data
    SystemFlags::instance->adcDoneMask.fetch_or(SystemFlags::POTS, std::memory_order_relaxed);
  }
  else if (hadc->Instance == ADC2)
  {
    // Done reading clutch temperature data
    SystemFlags::instance->adcDoneMask.fetch_or(SystemFlags::TEMP, std::memory_order_relaxed);
  }
  else if (hadc->Instance == ADC3)
  {
    auto lastADCSource = SystemFlags::instance->lastActiveADC.load();
    if (lastADCSource == SystemFlags::ADCSource::CLUTCH_3)
    {
      // Done sampling all clutch current data
      SystemFlags::instance->adcDoneMask.fetch_or(SystemFlags::CURR, std::memory_order_relaxed);
    }
  }
}
