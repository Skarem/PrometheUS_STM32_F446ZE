#include "TestsNamespace.hpp"

#include "main.h"

#include "PWM.hpp"
#include "PWMComplementary.hpp"

/**
 * @file  TestPwm.cpp
 * @brief Unit test mainly used for clutches PWM and complementary PWM outputs
 *
 * @details
 * This test initializes TIM1 (three complementary clutch PWMs) and TIM4 (motor PWM)
 * to verify correct duty-cycle generation and dead-time configuration.
 *
 * Add values in the DUTY_CYCLES array to add increments to the duty-cycle sweep
 *
 * TIM1_CH1   -> PE9    (Clutch_1_PWM_A)
 * TIM1_CH1N  -> PE8    (Clutch_1_PWM_B)
 * TIM1_CH2   -> PA9    (Clutch_2_PWM_A)
 * TIM1_CH2N  -> PE10   (Clutch_2_PWM_B)
 * TIM1_CH3   -> PA10   (Clutch_3_PWM_A)
 * TIM1_CH3N  -> PE12   (Clutch_3_PWM_B)
 *
 * TIM4_CH1   -> PB6    (Motor_PWM)
 */
void Tests::pwm()
{
  // PWM pwmMotor;
  // pwmMotor.init(&htim4, TIM_CHANNEL_1);
  // pwmMotor.start();
  // const float MOTOR_DUTY_CYCLE = 0.25f;
  // pwmMotor.update(MOTOR_DUTY_CYCLE);

  HAL_GPIO_WritePin(Clutch_1_PWM_B_GPIO_Port, Clutch_1_PWM_B_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Clutch_2_PWM_B_GPIO_Port, Clutch_2_PWM_B_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Clutch_3_PWM_B_GPIO_Port, Clutch_3_PWM_B_Pin, GPIO_PIN_RESET);

  PWMComplementary pwmClutch1;
  PWMComplementary pwmClutch2;
  PWMComplementary pwmClutch3;

  pwmClutch1.init(&htim1, TIM_CHANNEL_1);
  pwmClutch2.init(&htim1, TIM_CHANNEL_2);
  pwmClutch3.init(&htim1, TIM_CHANNEL_3);

  // const float DUTY_CYCLES[3] = { 0.10f };
  // const size_t NUM_DUTY = sizeof(DUTY_CYCLES) / sizeof(DUTY_CYCLES[0]);

  // const uint32_t STEP_TIME_MS = 1;
  // const uint32_t OFF_TIME_MS  = 10;

  const float DUTY_CYCLE_TEST = 0.10f;

  pwmClutch1.update(DUTY_CYCLE_TEST);
  pwmClutch2.update(DUTY_CYCLE_TEST);
  pwmClutch3.update(DUTY_CYCLE_TEST);

  pwmClutch1.start();
  pwmClutch2.start();
  pwmClutch3.start();

  while (true)
  {
	/*
    pwmClutch1.start();
    pwmClutch2.start();
    pwmClutch3.start();

    for (size_t i = 0; i < NUM_DUTY; ++i)
    {
      const float dutyCycle = DUTY_CYCLES[i];

      pwmClutch1.update(dutyCycle);
      pwmClutch2.update(dutyCycle);
      pwmClutch3.update(dutyCycle);

      HAL_Delay(STEP_TIME_MS);
    }

    pwmClutch1.stop();
    pwmClutch2.stop();
    pwmClutch3.stop();

    HAL_Delay(OFF_TIME_MS);
    */
	HAL_Delay(10);
  }
}
