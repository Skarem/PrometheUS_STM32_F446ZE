#include "cppMain.hpp"

#include "PrometheUS.hpp"
#include "TestsNamespace.hpp"

extern "C" {
#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
}

// ----- Running normal operation mode -----
#define RUN_INTERRUPT_CALLBACKS 0

// ----- Running unit tests -----
#if not RUN_INTERRUPT_CALLBACKS
// Need any specific callback sections
#define TEST_MOTOR          0               // Tests::motor
#define TEST_POTENTIOMETERS 0               // Tests::potentiometer
#define TEST_TEMPERATURES   0               // Tests::clutchTemperatures
#define TEST_CURRENTS       1               // Tests::clutchCurrents
#endif

// ========== Flags ==========
SystemFlags systemFlags;

// ========== Create data samplers (lowest-level globals) ==========
ClutchCurrentSampler clutchCurrentSampler1;
ClutchCurrentSampler clutchCurrentSampler2;
ClutchCurrentSampler clutchCurrentSampler3;
MotorVelocitySampler motorVelocitySampler;

void cppMain()
{
#if RUN_INTERRUPT_CALLBACKS
  PrometheUS_Gripper gripper(&systemFlags, &clutchCurrentSampler1, &clutchCurrentSampler2, &clutchCurrentSampler3, &motorVelocitySampler);
  gripper.init();

  while (true)
  {
    gripper.execute();
  }
#else
  cppTests();
#endif
}

void cppTests()
{
  // Tests::pwm();
  // Tests::motor(&systemFlags, &motorVelocitySampler);

  // Tests::sender();
  // Tests::senderErrors();

  // Tests::potentiometers(&systemFlags);
  // Tests::clutchTemperatures(&systemFlags);
  // Tests::clutchCurrents(&systemFlags, &clutchCurrentSampler1, &clutchCurrentSampler2, &clutchCurrentSampler3);

  // Tests::button();
  // Tests::buttonDebug();
  // Tests::LEDs();
  // Tests::LEDsDebug();

  // Tests::adcToVoltConversion();
  // Tests::adcToCurrConversion();
  // Tests::adcToTempConversion();

  Tests::stateMachines(&systemFlags, &clutchCurrentSampler1, &clutchCurrentSampler2, &clutchCurrentSampler3, &motorVelocitySampler);

  // Tests::PIDClutch(&systemFlags, &clutchCurrentSampler1, &clutchCurrentSampler2, &clutchCurrentSampler3);
}

extern "C" void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
#if RUN_INTERRUPT_CALLBACKS

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
#endif
#if TEST_MOTOR
  if (htim->Instance == TIM2)
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      SystemFlags::instance->lastActiveADC.store(SystemFlags::ADCSource::MOTOR, std::memory_order_relaxed);
      MotorVelocitySampler::instance->startSamplingRawAdcValue();
    }
  }
#endif
#if TEST_CURRENTS
  if (htim->Instance == TIM2)
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      SystemFlags::instance->updatePwmDutyCycles.store(true, std::memory_order_relaxed);
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      SystemFlags::instance->startControlCycle.store(true, std::memory_order_relaxed);
      SystemFlags::instance->lastActiveADC.store(SystemFlags::ADCSource::CLUTCH_1, std::memory_order_relaxed);
      ClutchCurrentSampler::instances[FINGER_1_INDEX]->startSamplingRawAdcValue();
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      SystemFlags::instance->lastActiveADC.store(SystemFlags::ADCSource::CLUTCH_2, std::memory_order_relaxed);
      ClutchCurrentSampler::instances[FINGER_2_INDEX]->startSamplingRawAdcValue();
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      SystemFlags::instance->lastActiveADC.store(SystemFlags::ADCSource::CLUTCH_3, std::memory_order_relaxed);
      ClutchCurrentSampler::instances[FINGER_3_INDEX]->startSamplingRawAdcValue();
    }
  }
#endif
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  // ----- Normal operation -----
#if (RUN_INTERRUPT_CALLBACKS || TEST_STATE_MACHINES)
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
#endif
  // ----- Tests -----
#if TEST_MOTOR
  if (hadc->Instance == ADC3)
  {
    auto lastADCSource = SystemFlags::instance->lastActiveADC.load();
    if (lastADCSource == SystemFlags::ADCSource::MOTOR)
    {
      SystemFlags::instance->motorVelocityDone.store(true, std::memory_order_relaxed);
    }
  }
#endif
#if TEST_POTENTIOMETERS
  if (hadc->Instance == ADC1)
  {
    SystemFlags::instance->adcDoneMask.fetch_or(SystemFlags::POTS, std::memory_order_relaxed);
  }
#endif
#if TEST_TEMPERATURES
  if (hadc->Instance == ADC2)
  {
    SystemFlags::instance->adcDoneMask.fetch_or(SystemFlags::TEMP, std::memory_order_relaxed);
  }
#endif
#if TEST_CURRENTS
  if (hadc->Instance == ADC3)
  {
    auto lastADCSource = SystemFlags::instance->lastActiveADC.load();
    if (lastADCSource == SystemFlags::ADCSource::CLUTCH_3)
    {
      SystemFlags::instance->adcDoneMask.fetch_or(SystemFlags::CURR, std::memory_order_relaxed);
    }
    else if (lastADCSource == SystemFlags::ADCSource::MOTOR)
    {
      SystemFlags::instance->motorVelocityDone.store(true, std::memory_order_relaxed);
    }
  }
#endif
}
