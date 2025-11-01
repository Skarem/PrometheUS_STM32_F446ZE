#include "Tests.hpp"

#include "main.h"

#include "PWM.hpp"
#include "PWMComplementary.hpp"

void Tests::pwm()
{
  // PWM pwmMotor;
  PWMComplementary pwmClutch1;
  // PWMComplementary pwmClutch2;
  // PWMComplementary pwmClutch3;

  // pwmMotor.init(&htim4, TIM_CHANNEL_1);
  pwmClutch1.init(&htim1, TIM_CHANNEL_1);
  // pwmClutch2.init(&htim1, TIM_CHANNEL_2);
  // pwmClutch3.init(&htim1, TIM_CHANNEL_3);

  // const float MOTOR_DUTY_CYCLE = 0.5f;
  // pwmMotor.update(MOTOR_DUTY_CYCLE);

  const float DUTY_CYCLES[3] = { 0.50f };
  const size_t NUM_DUTY = sizeof(DUTY_CYCLES) / sizeof(DUTY_CYCLES[0]);

  const uint32_t STEP_TIME_MS = 1;
  const uint32_t OFF_TIME_MS  = 10;

  while (true)
  {
    pwmClutch1.start();
    // pwmClutch2.start();
    // pwmClutch3.start();

    for (size_t i = 0; i < NUM_DUTY; ++i)
    {
      const float dutyCycle = DUTY_CYCLES[i];

      pwmClutch1.update(dutyCycle);
      // pwmClutch2.update(dutyCycle);
      // pwmClutch3.update(dutyCycle);

      HAL_Delay(STEP_TIME_MS);
    }

    pwmClutch1.stop();
    // pwmClutch2.stop();
    // pwmClutch3.stop();

    HAL_Delay(OFF_TIME_MS);
  }
}
