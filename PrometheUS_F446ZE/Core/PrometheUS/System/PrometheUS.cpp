#include "PrometheUS.hpp"

#include <cstdio>

void PrometheUS_Gripper::printConsole(const char message[], uint16_t messageLength)
{
  if (verbose)
  {
    HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(message), messageLength, HAL_MAX_DELAY);
  }
}

PrometheUS_Gripper::PrometheUS_Gripper(
    SystemFlags* systemFlags,
    ClutchCurrentSampler* clutchCurrentSampler1,
    ClutchCurrentSampler* clutchCurrentSampler2,
    ClutchCurrentSampler* clutchCurrentSampler3,
    MotorVelocitySampler* motorVelocitySampler)
  : m_systemFlags(systemFlags),
    m_finger1(clutchCurrentSampler1),
    m_finger2(clutchCurrentSampler2),
    m_finger3(clutchCurrentSampler3),
    m_motor(motorVelocitySampler)
{

}

void PrometheUS_Gripper::init()
{
  m_telemetrySender.init(&huart3);

  m_ledRun.init(DEL_1_GPIO_Port, DEL_1_Pin);
  m_ledError.init(DEL_2_GPIO_Port, DEL_2_Pin);
  m_ledRun.off();
  m_ledError.off();

  m_debugPin.init(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
  m_debugPin.off();

  m_button.init(Button_Software_Run_GPIO_Port, Button_Software_Run_Pin);

  m_potentiometers.init(&hadc1);
  m_clutchesTemperature.init(&hadc2);

  m_finger1.init(ADC_CHANNEL_4, FINGER_1_INDEX, Encoder_CS_1_GPIO_Port, Encoder_CS_1_Pin, TIM_CHANNEL_1, Clutch_1_PWM_B_GPIO_Port, Clutch_1_PWM_B_Pin);
  m_finger2.init(ADC_CHANNEL_5, FINGER_2_INDEX, Encoder_CS_2_GPIO_Port, Encoder_CS_2_Pin, TIM_CHANNEL_2, Clutch_2_PWM_B_GPIO_Port, Clutch_2_PWM_B_Pin);
  m_finger3.init(ADC_CHANNEL_6, FINGER_3_INDEX, Encoder_CS_3_GPIO_Port, Encoder_CS_3_Pin, TIM_CHANNEL_3, Clutch_3_PWM_B_GPIO_Port, Clutch_3_PWM_B_Pin);

  m_motor.init();

  startTimers();
}

void PrometheUS_Gripper::execute()
{
  systemStateMachine();
}

void PrometheUS_Gripper::systemStateMachine()
{
  switch (m_systemState)
  {
    case SystemState::SYS_INIT:
      m_finger1.clutch0RPM();
      m_finger2.clutch0RPM();
      m_finger3.clutch0RPM();

      m_systemState = SystemState::SYS_IDLE;
      break;

    case SystemState::SYS_IDLE:
      doIdle();
      break;

    case SystemState::SYS_RUN:
      doRun();
      break;

    case SystemState::SYS_ERROR:
      doError();
      break;
  }
}

void PrometheUS_Gripper::doIdle()
{
  //  Sample potentiometers, clutch temperatures, motor velocity
  sampleStateMachine();

  // If switch is pressed => switch to RUN
  if (m_button.pressed())
  {
    static const char msg[] = "GO TO RUN\r\n";
    printConsole(msg, sizeof(msg));

    start();
    m_ledRun.on();

    m_controlState = ControlState::CONTROL_START_SAMPLING;
    m_systemState = SystemState::SYS_RUN;
  }
}

void PrometheUS_Gripper::doRun()
{
  // If any error is detected, switch to ERROR
  if (m_systemFlags->errorDetected.exchange(false))
  {
    static const char msg[] = "GO TO ERROR\r\n";
    printConsole(msg, sizeof(msg));

    stop();
    m_ledRun.off();
    m_ledError.on();

    m_finger1.clutch0RPM();
    m_finger2.clutch0RPM();
    m_finger3.clutch0RPM();

    m_motorVelocity = 0;

    m_systemState = SystemState::SYS_ERROR;
  }
  // If switch is un-toggled => switch to IDLE
  else if (m_button.pressed())
  {
    static const char msg[] = "GO TO IDLE\r\n";
    printConsole(msg, sizeof(msg));

    stop();
    m_ledRun.off();

    m_finger1.clutch0RPM();
    m_finger2.clutch0RPM();
    m_finger3.clutch0RPM();

    m_motorVelocity = 0;

    m_systemState = SystemState::SYS_IDLE;
  }
  else
  {
    controlStateMachine();
  }
}

void PrometheUS_Gripper::doError()
{
  // Send error message with error source
  if (m_sendErrorMessage)
  {
    m_sendErrorMessage = false;
    if (visualize)
    {
      uint32_t now = HAL_GetTick();
      m_telemetrySender.send(
          now,
          m_potentiometerPercentagesArray,
          m_clutchTemperaturesArray,
          m_clutchDutyCyclesArray,
          m_motorVelocity,
          true,
          static_cast<uint8_t>(m_errorSource)
      );
    }
  }
  else
  {
	sampleStateMachine();

	// Clean up flags
    m_errorFlags.clearFlags();
    m_errorSource = ErrorSource::NONE;
    m_systemFlags->errorDetected.store(false, std::memory_order_relaxed);

    checkForErrors(false);
    bool errorDetected = m_errorFlags.anyError();

    // Stay in error until the switch is turned off
    if (m_button.pressed())
    {
      // ----- Check if still in error -----

      // Still in error - send a message with the error source
      if (errorDetected)
      {
        m_sendErrorMessage = true;

        static char msg[32];
        int len = snprintf(msg, sizeof(msg), "STILL ERROR : %d\r\n", static_cast<uint8_t>(m_errorSource));
        printConsole(msg, len);
      }
      // Not in error anymore
      else
      {
        const char msg[] = "NO ERROR - GO TO IDLE\r\n";
        printConsole(msg, sizeof(msg));

        m_ledError.off();

        m_controlState = ControlState::CONTROL_START_SAMPLING;
        m_systemState = SystemState::SYS_IDLE;
      }
    }
  }
}

void PrometheUS_Gripper::controlStateMachine()
{
  switch (m_controlState)
  {
    case ControlState::CONTROL_START_SAMPLING:
      doStartSampling();
      break;

    case ControlState::CONTROL_WAIT_SAMPLING:
      doWaitSampling();
      break;

    case ControlState::CONTROL_CALCULATE_CONTROL_LAWS:
      doCalculate();
      break;

    case ControlState::CONTROL_FINISHED_CONTROL_LAWS:
      doFinishedControlLaws();
      break;

    case ControlState::CONTROL_UPDATE_PWM_DUTY_CYCLES:
      doUpdatePWM();
      break;

    case ControlState::CONTROL_FINISHED_CYCLE:
      doFinishedCycle();
      break;
  }
};

void PrometheUS_Gripper::doStartSampling()
{
  // NOTE: All ADC sampling is triggered by hardware timers configured in .ioc
  // m_debugPin.pulse();

  // Reset the mask testing for sampling completion
  m_systemFlags->adcDoneMask.store(0, std::memory_order_relaxed);

  m_controlState = ControlState::CONTROL_WAIT_SAMPLING;
}

void PrometheUS_Gripper::doWaitSampling()
{
  // Waiting for all ADC transmissions to be done
  uint8_t mask = m_systemFlags->adcDoneMask.load(std::memory_order_relaxed);

  if ((mask & SystemFlags::ALL) == SystemFlags::ALL)
  {
    m_controlState = ControlState::CONTROL_CALCULATE_CONTROL_LAWS;
  }
}

void PrometheUS_Gripper::doCalculate()
{
  // m_debugPin.on();

  // Convert raw ADC values into engineering units
  readSensors();

  // Send info at low rate to visualize on computer
  maybeSendTelemetry();

  // Check for errors
  computeErrors();

  // Control laws
  computeControlLaws();

  // m_debugPin.off();

  // Reset the flag waiting to update the PWM duty cycles
  m_systemFlags->updatePwmDutyCycles.store(false, std::memory_order_relaxed);

  m_controlState = ControlState::CONTROL_FINISHED_CONTROL_LAWS;
}

void PrometheUS_Gripper::doFinishedControlLaws()
{
  // Waiting for the timer trigger to update PWM duty cycles
  if (m_systemFlags->updatePwmDutyCycles.exchange(false))
  {
    m_controlState = ControlState::CONTROL_UPDATE_PWM_DUTY_CYCLES;
  }
}

void PrometheUS_Gripper::doUpdatePWM()
{
  // m_debugPin.pulse();

  // Update PWM duty cycles with new values calculated in CONTROLE_CALCULATE_LAWS via burst DMA
  m_finger1.updateCommand();
  m_finger2.updateCommand();
  m_finger3.updateCommand();

  m_controlState = ControlState::CONTROL_FINISHED_CYCLE;
}

void PrometheUS_Gripper::doFinishedCycle()
{
  // Done updating, cycle completed

  // Waiting for timer trigger to start next cycle
  if (m_systemFlags->startControlCycle.exchange(false))
  {
    m_controlState = ControlState::CONTROL_START_SAMPLING;
  }
}

void PrometheUS_Gripper::readSensors()
{
  // Convert potentiometer from ADC to percentage [0.0 - 100.0]
  m_potentiometers.convertAll(m_potentiometerPercentagesArray);

  // Convert temperature from ADC to Celsius
  m_clutchesTemperature.convertAll(m_clutchTemperaturesArray);

  // Convert potentiometer to duty cycle
  mapPotentiometersToDutyCycleWithDeadZones();

  static uint8_t counterPots = 0;
  static char msgPots[64];

  if (++counterPots % 100 == 0)
  {
    int len = snprintf(msgPots, sizeof(msgPots), "%.2f %.2f %.2f\r\n",
  	        m_clutchDutyCyclesArray[FINGER_1_INDEX],
			m_clutchDutyCyclesArray[FINGER_2_INDEX],
			m_clutchDutyCyclesArray[FINGER_3_INDEX]);
  	printConsole(msgPots, len);
  }

  // Convert motor velocity from ADC to RPM
  // m_motorVelocity = m_motor.getMotorVelocity();

  /*
  static int counterMotor = 0;
  static char txBuf[16];

  if (++counterMotor % 100 == 0)
  {
    int len = snprintf(txBuf, sizeof(txBuf), "%.2f\r\n", m_motorVelocity);
    printConsole(txBuf, len);
  }
  */
}

void PrometheUS_Gripper::maybeSendTelemetry()
{
  // Send data at low frequency
  if (visualize)
  {
    if (m_sendDataCounter++ >= SEND_DATA_TIMING)
    {
      m_sendDataCounter = 0;
      uint32_t now = HAL_GetTick();

      m_telemetrySender.send(
          now,
          m_potentiometerPercentagesArray,
          m_clutchTemperaturesArray,
          m_clutchDutyCyclesArray,
          m_motorVelocity,
          false,
          static_cast<uint8_t>(ErrorSource::NONE)
      );
    }
  }
}

void PrometheUS_Gripper::computeErrors()
{
  checkForErrors(true);
  bool errorDetected = m_errorFlags.anyError();
  m_systemFlags->errorDetected.store(errorDetected, std::memory_order_relaxed);
}

void PrometheUS_Gripper::computeControlLaws()
{
  // Clutch control
  m_finger1.calculateCommand(m_clutchDutyCyclesArray[FINGER_1_INDEX]);
  m_finger2.calculateCommand(m_clutchDutyCyclesArray[FINGER_2_INDEX]);
  m_finger3.calculateCommand(m_clutchDutyCyclesArray[FINGER_3_INDEX]);

  // Motor control
  uint8_t activeCount = 0;
  for (size_t i = 0; i < FINGER_COUNT; ++i)
  {
    if (m_clutchDutyCyclesArray[i] > 0.0f)
    {
      activeCount++;
    }
  }

  float newTargetRPM = BASE_MOTOR_VELOCITY + (activeCount * RPM_INCREASE_PER_FINGER_MOVING);
  if (newTargetRPM != m_oldTargetRPM)
  {
    m_motor.setRPM(newTargetRPM);
    m_oldTargetRPM = newTargetRPM;
  }
  m_motorVelocity = newTargetRPM;
}

void PrometheUS_Gripper::mapPotentiometersToDutyCycleWithDeadZones()
{
  // Duty cycle range [0.0 - 1.0]
  const float DUTY_MIN_USEFUL = 0.30f;
  const float DUTY_MAX_USEFUL = 0.57f;

  // Potentiometer thresholds in percent [0.0 - 100.0]
  const float POT_LOW_DEADZONE  = 20.0f;
  const float POT_HIGH_DEADZONE = 80.0f;

  for (size_t index = 0; index < FINGER_COUNT; ++index)
  {
    float dutyCycle = 0.0f;
    float pot = m_potentiometerPercentagesArray[index];

    if (pot <= POT_LOW_DEADZONE)
    {
      dutyCycle = 0.0f;
    }
    else if (pot >= POT_HIGH_DEADZONE)
    {
      dutyCycle = DUTY_MAX_USEFUL;
    }
    else
    {
      // Normalize potentiometer from [20.0 - 80.0] to [0.0 - 1.0]
      float activeRegion = (pot - POT_LOW_DEADZONE) / (POT_HIGH_DEADZONE - POT_LOW_DEADZONE);

      // Map to duty cycle range
      dutyCycle = DUTY_MIN_USEFUL + activeRegion * (DUTY_MAX_USEFUL - DUTY_MIN_USEFUL);
    }
    m_clutchDutyCyclesArray[index] = dutyCycle;
  }
}

void PrometheUS_Gripper::startTimers()
{
  // ========== TIM2 ==========
  // Is used as the main timer to synchronize the events of the state machine
  // The main control loop runs at 1KHz, this timer triggers 10 times per 1KHz
  // The triggers are synchronized to be in the middle of the high pulse of the PWM signals

  // Pulse 1/10
  // ADC DMA is automatically triggered by OC2REF
  // Potentiometers       (ADC1 IN0, IN1, IN2)
  // Temperature sensors  (ADC2 IN7, IN8, IN9)
  // OC2REF triggers a function callback, in which ADC DMA for clutch current #1 is started (ADC3 IN4)
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2);

  // Pulse 2/10
  // OC3REF triggers a function callback, in which ADC DMA for clutch current #2 is started (ADC3 IN5)
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3);

  // Pulse 3/10
  // OC4REF triggers a function callback, in which ADC DMA for clutch current #3 is started (ADC3 IN6)
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_4);

  // Pulse 10/10
  // OC1REF triggers a function callback, in which :
  // ADC DMA for the motor velocity is started (ADC3 IN7)
  // The PWM duty cycles for the clutches are updated
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);

  // ========== TIM3 ==========
  // Is an "intermediate" timer that TIM2 uses.
  // TIM3 triggers twice, at the middle of the high pulse of TIM1
  HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);
}

void PrometheUS_Gripper::start()
{
  static const char startMsg[] = "Start\r\n";
  printConsole(startMsg, sizeof(startMsg));

  m_finger1.start();
  m_finger2.start();
  m_finger3.start();

  m_motor.setRPM(BASE_MOTOR_VELOCITY);
  m_motor.start();
}

void PrometheUS_Gripper::stop()
{
  static const char stopMsg[] = "Stop\r\n";
  printConsole(stopMsg, sizeof(stopMsg));

  m_finger1.stop();
  m_finger2.stop();
  m_finger3.stop();

  m_motor.setRPM(0);
  m_motor.stop();
}

void PrometheUS_Gripper::checkForErrors(bool useFiltering)
{
  // Clutch temperature error
  for (size_t index = 0; index < FINGER_COUNT; ++index)
  {
    const float temp = m_clutchTemperaturesArray[index];
    const bool isBad = (temp < MIN_CLUTCH_TEMPERATURE_CELSIUS) || (temp > MAX_CLUTCH_TEMPERATURE_CELSIUS);

    if (useFiltering)
    {
      // Normal mode : multi-samples
      if (isBad)
      {
        m_tempErrorCounters[index]++;

        if (m_tempErrorCounters[index] >= TEMP_ERROR_COUNT_THRESHOLD)
        {
          m_errorFlags.clutchTemperatures[index] = true;
          setErrorSource(index);
          return;
        }
      }
      else
      {
        m_tempErrorCounters[index] = 0;
      }
    }
    else
    {
      // Reset mode : single-sample hard check
      if (isBad)
      {
        m_errorFlags.clutchTemperatures[index] = true;
        setErrorSource(index);
        return;
      }
    }
  }

  // Motor error
  if (m_motor.isError())
  {
    m_errorFlags.motor = true;
    m_errorSource = ErrorSource::MOTOR_ERROR_PIN;
  }
}

inline void PrometheUS_Gripper::setErrorSource(size_t index)
{
  switch (index)
  {
    case FINGER_1_INDEX:
      m_errorSource = ErrorSource::CLUTCH_TEMPERATURE_1;
      break;
    case FINGER_2_INDEX:
      m_errorSource = ErrorSource::CLUTCH_TEMPERATURE_2;
      break;
    case FINGER_3_INDEX:
      m_errorSource = ErrorSource::CLUTCH_TEMPERATURE_3;
      break;
  }
}

void PrometheUS_Gripper::sampleStateMachine()
{

  switch (m_sampleState)
  {
    case CONTROL_WAIT_SAMPLING:
    {
	  // static char msg[] = "SAMPLE\r\n";
	  // printConsole(msg, sizeof(msg));

	  uint8_t mask = m_systemFlags->adcDoneMask.load(std::memory_order_relaxed);

      if((mask & SystemFlags::READ_ONLY) == SystemFlags::READ_ONLY)
	  {
    	m_systemFlags->adcDoneMask.store(0, std::memory_order_relaxed);
        m_sampleState = ControlState::CONTROL_CALCULATE_CONTROL_LAWS;
	  }
	  break;
    }
    case CONTROL_CALCULATE_CONTROL_LAWS:
    {
      // static char msg[] = "CONVERT\r\n";
      // printConsole(msg, sizeof(msg));

      // Read potentiometers and clutch temperatures
      readSensors();
      maybeSendTelemetry();

      m_sampleState = ControlState::CONTROL_WAIT_SAMPLING;
      break;
    }
  }
}
