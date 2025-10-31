#include "Tests.hpp"

#include "main.h"

#include "PWM.hpp"
#include "PWMComplementary.hpp"

void Tests::pwm()
{
  // TIM4 is synchronized with TIM1, so motor PWM won't work
  // without at least one clutch PWM active (TIM1 must be running)
  PWM pwmMotor;
  PWMComplementary pwmClutch1;
  PWMComplementary pwmClutch2;
  PWMComplementary pwmClutch3;

  // Initialize all PWMs
  pwmMotor.init(&htim4, TIM_CHANNEL_1);
  pwmClutch1.init(&htim1, TIM_CHANNEL_1);
  pwmClutch2.init(&htim1, TIM_CHANNEL_2);
  pwmClutch3.init(&htim1, TIM_CHANNEL_3);

  // Start all PWMs initially
  pwmMotor.start();
  pwmClutch1.start();
  pwmClutch2.start();
  pwmClutch3.start();

  const float MOTOR_DUTY_CYCLE = 0.5f;
  pwmMotor.update(MOTOR_DUTY_CYCLE);

  const float MIN_DUTY_CYCLE = 0.0f;
  const float MAX_DUTY_CYCLE = 1.0f;

  const float STEP = 0.1f;      // Increment per update
  const uint32_t DELAY_MS = 1;  // Delay per step
  const uint32_t TOTAL_STEPS = static_cast<uint32_t>(MAX_DUTY_CYCLE / STEP);

  float dutyCycle = 0.0f;
  uint32_t stepCount = 0;
  bool running = true;

  while (true)
  {
    if (running)
    {
      // --- Active sweep ---
      dutyCycle += STEP;
      if (dutyCycle > MAX_DUTY_CYCLE)
      {
        dutyCycle = MIN_DUTY_CYCLE;
      }

      pwmClutch1.update(dutyCycle);
      pwmClutch2.update(dutyCycle);
      pwmClutch3.update(dutyCycle);

      stepCount++;
      if (stepCount >= TOTAL_STEPS)
      {
        // Finished one full sweep - stop all clutches
        pwmClutch1.stop();
        pwmClutch2.stop();
        pwmClutch3.stop();
        running = false;
        stepCount = 0;
      }
    }
    else
    {
      // --- Stopped period ---
      stepCount++;
      if (stepCount >= TOTAL_STEPS)
      {
        // Finished the "stop" phase - restart all clutches
        pwmClutch1.start();
        pwmClutch2.start();
        pwmClutch3.start();
        dutyCycle = MIN_DUTY_CYCLE;
        running = true;
        stepCount = 0;
      }
    }
    HAL_Delay(DELAY_MS);
  }
}
