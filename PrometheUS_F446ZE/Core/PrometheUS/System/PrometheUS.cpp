#include "PrometheUS.hpp"

PrometheUS_Gripper::PrometheUS_Gripper(
    SystemFlags &systemFlags,
    ClutchCurrentSampler &clutchCurrentSampler1,
    ClutchCurrentSampler &clutchCurrentSampler2,
    ClutchCurrentSampler &clutchCurrentSampler3,
    MotorVelocitySampler &motorVelocitySampler)
  : m_systemFlags(systemFlags),
    m_motor(motorVelocitySampler)
{
  new (&m_fingers[FINGER_1_INDEX]) Finger(clutchCurrentSampler1);
  new (&m_fingers[FINGER_2_INDEX]) Finger(clutchCurrentSampler2);
  new (&m_fingers[FINGER_3_INDEX]) Finger(clutchCurrentSampler3);
}

void PrometheUS_Gripper::init()
{
  // __disable_irq();

  startTimers();

  m_telemetrySender.init(&huart3);

  m_led1.init(DEL_1_GPIO_Port, DEL_1_Pin);
  m_led2.init(DEL_2_GPIO_Port, DEL_2_Pin);

  m_switch.init(SWRUN_GPIO_Port, SWRUN_Pin);

  m_potentiometers.init(&hadc1);
  m_clutchesTemperature.init(&hadc2);

  m_fingers[FINGER_1_INDEX].init(ADC_CHANNEL_4, FINGER_1_INDEX, Encoder_CS_1_GPIO_Port, Encoder_CS_1_Pin, TIM_CHANNEL_1);
  m_fingers[FINGER_2_INDEX].init(ADC_CHANNEL_5, FINGER_2_INDEX, Encoder_CS_2_GPIO_Port, Encoder_CS_2_Pin, TIM_CHANNEL_2);
  m_fingers[FINGER_3_INDEX].init(ADC_CHANNEL_6, FINGER_3_INDEX, Encoder_CS_3_GPIO_Port, Encoder_CS_3_Pin, TIM_CHANNEL_3);

  m_motor.init();

  start();

  // __enable_irq();
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
    {
      // Initialize all objects here ...

      m_systemState = SystemState::SYS_IDLE;
      break;
    }
    case SystemState::SYS_IDLE:
    {
      // If switch is toggled => switch to RUN
      // if (m_switch.isOn())
      // {
        m_systemState = SystemState::SYS_RUN;
      // }
      break;
    }
    case SystemState::SYS_RUN:
    {
      // If any error is detected, switch to ERROR
      // if (m_systemFlags.errorDetected.exchange(false))
      // {
      //   m_systemState = SystemState::SYS_ERROR;
      // }
      // If switch is un-toggled => switch to IDLE
      // else if (!m_switch.isOn())
      // {
      //   m_systemState = SystemState::SYS_IDLE;
      // }
      // else
      // {
        controlStateMachine();
      // }
      break;
    }
    case SystemState::SYS_ERROR:
    {
      // Send error message with error source (from CheckIfError)
      if (m_sendErrorMessage)
      {
        m_sendErrorMessage = false;
        uint32_t now = HAL_GetTick();
        m_telemetrySender.send(now, m_potentiometerPositionsArray, m_clutchTemperaturesArray, m_clutchCurrentsArray, m_motorVelocity, true, static_cast<uint8_t>(m_errorSource));
      }
      else
      {
        // Stay in error until the switch is turned off
        if (m_switch.isOn())
        {
          m_systemState = SystemState::SYS_IDLE;

          // TODO: Cleanup to start the system again
        }
      }
      break;
    }
  }
}

void PrometheUS_Gripper::controlStateMachine()
{
  switch (m_controlState)
  {
    case ControlState::CONTROL_START_SAMPLING:
    {
      // NOTE: All ADC sampling is triggered by hardware timers configured in .ioc
      // Sample encoders
      // for (size_t index = 0; index < FINGER_COUNT; ++index)
      // {
      //   m_fingers[index].sampleEncoder();
      // }
      m_controlState = ControlState::CONTROL_WAIT_SAMPLING;
      break;
    }
    case ControlState::CONTROL_WAIT_SAMPLING:
    {
      // Waiting for all ADC transmissions to be done
      uint8_t mask = m_systemFlags.adcDoneMask.load(std::memory_order_relaxed);
      if ((mask & SystemFlags::ALL) == SystemFlags::ALL)
      {
        m_controlState = ControlState::CONTROL_CALCULATE_CONTROL_LAWS;
      }
      break;
    }
    case ControlState::CONTROL_CALCULATE_CONTROL_LAWS:
    {
      // Convert potentiometer from ADC to degrees
      m_potentiometers.convertAll(m_potentiometerPositionsArray);
      // Convert current from ADC to Amperes
      for (size_t index = 0; index < FINGER_COUNT; ++index)
      {
        m_clutchCurrentsArray[index] = m_fingers[index].getClutchMeasuredCurrent();
      }
      // Convert temperature from ADC to Celsius
      m_clutchesTemperature.convertAll(m_clutchTemperaturesArray);

      uint32_t now = HAL_GetTick();
      m_motorVelocity = m_motor.getMotorVelocity();

      // Send data @ 50 Hz
      if (++m_sendDataCounter >= SEND_DATA_TIMING)
      {
        m_sendDataCounter = 0;
        m_telemetrySender.send(now, m_potentiometerPositionsArray, m_clutchCurrentsArray, m_clutchCurrentsArray, m_motorVelocity, false, static_cast<uint8_t>(ErrorSource::NONE));
      }

      // Check for errors
      checkForErrors();
      bool errorDetected = m_errorFlags.anyError();
      m_systemFlags.errorDetected.store(errorDetected, std::memory_order_relaxed);

      // Control laws
      for (size_t index = 0; index < FINGER_COUNT; ++index)
      {
        m_fingers[index].calculateCommand(m_potentiometerPositionsArray[index]);
      }

      m_controlState = ControlState::CONTROL_FINISHED_CONTROL_LAWS;
      break;
    }
    case ControlState::CONTROL_FINISHED_CONTROL_LAWS:
    {
      // Waiting for the timer trigger to update PWM duty cycles
      if (m_systemFlags.updatePwmDutyCycles.exchange(false))
      {
        m_controlState = ControlState::CONTROL_UPDATE_PWM_DUTY_CYCLES;
      }
      break;
    }
    case ControlState::CONTROL_UPDATE_PWM_DUTY_CYCLES:
    {
      // Error detected
      // if (m_systemFlags.errorDetected.load())
      // {
      //   stop();
      // }
      // Update PWM duty cycles with new values calculated in CONTROLE_CALCULATE_LAWS via burst DMA
      // else
      // {
        for (size_t index = 0; index < FINGER_COUNT; ++index)
        {
          m_fingers[index].updateCommand();
        }
      // }

      m_controlState = ControlState::CONTROL_FINISHED_CYCLE;
      break;
    }
    case ControlState::CONTROL_FINISHED_CYCLE:
    {
      // Done updating, cycle completed
      // Waiting for timer trigger to start next cycle
      if (m_systemFlags.startControlCycle.exchange(false))
      {
        m_controlState = ControlState::CONTROL_START_SAMPLING;
      }
      break;
    }
  }
};

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
  for (size_t index = 0; index < FINGER_COUNT; ++index)
  {
    m_fingers[index].start();
  }
  m_motor.start();
}

void PrometheUS_Gripper::stop()
{
  for (size_t index = 0; index < FINGER_COUNT; ++index)
  {
    m_fingers[index].stop();
  }
  m_motor.stop();
}

void PrometheUS_Gripper::checkForErrors()
{
  for (size_t index = 0; index < FINGER_COUNT; ++index)
  {
    // ----- Clutch temperatures -----
    if (m_clutchTemperaturesArray[index] < MIN_CLUTCH_TEMPERATURE_CELSIUS ||
        m_clutchTemperaturesArray[index] > MAX_CLUTCH_TEMPERATURE_CELSIUS)
    {
      m_errorFlags.clutchTemperatures[index] = true;

      switch (index)
      {
        case FINGER_1_INDEX: m_errorSource = ErrorSource::CLUTCH_TEMPERATURE_1; break;
        case FINGER_2_INDEX: m_errorSource = ErrorSource::CLUTCH_TEMPERATURE_2; break;
        case FINGER_3_INDEX: m_errorSource = ErrorSource::CLUTCH_TEMPERATURE_3; break;
      }
    }
    // ----- Clutch currents -----
    if (m_fingers[index].isClutchError())
    {
      m_errorFlags.clutchCurrents[index] = true;

      switch (index)
      {
        case FINGER_1_INDEX: m_errorSource = ErrorSource::CLUTCH_CURRENT_1; break;
        case FINGER_2_INDEX: m_errorSource = ErrorSource::CLUTCH_CURRENT_2; break;
        case FINGER_3_INDEX: m_errorSource = ErrorSource::CLUTCH_CURRENT_3; break;
      }
    }
  }

  // ----- Motor error -----
  if (m_motor.isError())
  {
    m_errorSource = ErrorSource::MOTOR_ERROR_PIN;
  }
}
