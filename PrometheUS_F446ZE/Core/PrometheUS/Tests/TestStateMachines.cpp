#include "TestsNamespace.hpp"

#include "DebugButton.hpp"
#include "PrometheUS.hpp"

#include <string>

static void printToConsole(std::string msg)
{
  HAL_UART_Transmit(&huart3, (uint8_t*)msg.c_str(), msg.length(), HAL_MAX_DELAY);
}

class TestStateMachines
{
public:
  TestStateMachines(
      SystemFlags* systemFlags,
      ClutchCurrentSampler* clutch1,
      ClutchCurrentSampler* clutch2,
      ClutchCurrentSampler* clutch3,
      MotorVelocitySampler* motor)
    : m_systemFlags(systemFlags),
      m_clutch1(clutch1),
      m_clutch2(clutch2),
      m_clutch3(clutch3),
      m_motor(motor)
  {

  }

  ~TestStateMachines()  = default;

  void init()
  {
    PrometheUS_Gripper::startTimers();

    m_button.init(DEBUG_BUTTON_GPIO_Port, DEBUG_BUTTON_Pin);

    m_ledRun.init(LED_Debug_Run_GPIO_Port, LED_Debug_Run_Pin);
    m_ledError.init(LED_Debug_Error_GPIO_Port, LED_Debug_Error_Pin);

    m_debugPin.init(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);

    m_ledRun.off();
    m_ledError.off();

    m_debugPin.off();

    m_errorPin.init(Motor_Error_GPIO_Port, Motor_Error_Pin);

    m_clutch1->init(&hadc3, ADC_CHANNEL_4, FINGER_1_INDEX);
    m_clutch1->init(&hadc3, ADC_CHANNEL_5, FINGER_2_INDEX);
    m_clutch1->init(&hadc3, ADC_CHANNEL_6, FINGER_3_INDEX);

    m_motor->init(&hadc3, ADC_CHANNEL_7);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  }

  void execute()
  {
    systemStateMachine();
  }

private:

  SystemFlags*  m_systemFlags;

  DebugButton   m_button;

  DigitalOutput m_ledRun;
  DigitalOutput m_ledError;

  // Simulate motor error
  DigitalInput  m_errorPin;

  DigitalOutput m_debugPin;

  SystemState   m_systemState = SystemState::SYS_INIT;
  ControlState  m_controlState = ControlState::CONTROL_START_SAMPLING;

  ClutchCurrentSampler* m_clutch1;
  ClutchCurrentSampler* m_clutch2;
  ClutchCurrentSampler* m_clutch3;
  MotorVelocitySampler* m_motor;

  void systemStateMachine()
  {
    switch (m_systemState)
    {
      case SystemState::SYS_INIT:
      {
        // Initialize all objects here
        // TODO: Not really needed ...
        m_systemState = SystemState::SYS_IDLE;

        break;
      }
      case SystemState::SYS_IDLE:
      {
        // If change in button state is detected => RUN
        if (m_button.pressed())
        {
          m_ledRun.on();
          m_controlState = ControlState::CONTROL_START_SAMPLING;
          m_systemState = SystemState::SYS_RUN;
        }
        break;
      }
      case SystemState::SYS_RUN:
      {
        // If any error is detected in controlStateMachine from sampling, switch to ERROR
        if (m_systemFlags->errorDetected.exchange(false))
        {
          m_systemState = SystemState::SYS_ERROR;
        }
        // If change in button state is detected => IDLE
        else if (m_button.pressed())
        {
          m_ledRun.off();
          m_systemState = SystemState::SYS_IDLE;
        }
        else
        {
          controlStateMachine();
        }
        break;
      }
      case SystemState::SYS_ERROR:
      {
        // TODO: Handle error later
        m_ledError.off();
        break;
      }
    }
  }

  void controlStateMachine()
  {
    switch (m_controlState)
    {
      case ControlState::CONTROL_START_SAMPLING:
      {
        // NOTE: All ADC sampling is triggered by hardware timers configured in .ioc
        m_debugPin.pulse();
        // printToConsole("1\r\n");
        m_controlState = ControlState::CONTROL_WAIT_SAMPLING;
        break;
      }
      case ControlState::CONTROL_WAIT_SAMPLING:
      {
        // Waiting for all ADC transmissions to be done
        uint8_t maskCurrOnly = m_systemFlags->adcDoneMask.load(std::memory_order_relaxed);
        if (maskCurrOnly & SystemFlags::CURR)
        {
          m_debugPin.pulse();
          // printToConsole("2\r\n");
          m_controlState = ControlState::CONTROL_CALCULATE_CONTROL_LAWS;
        }
        break;
      }
      case ControlState::CONTROL_CALCULATE_CONTROL_LAWS:
      {
        // Check for error
        bool errorDetected = checkForError();
        m_systemFlags->errorDetected.store(errorDetected, std::memory_order_relaxed);

        m_debugPin.pulse();
        // printToConsole("3\r\n");
        m_controlState = ControlState::CONTROL_FINISHED_CONTROL_LAWS;
        break;
      }
      case ControlState::CONTROL_FINISHED_CONTROL_LAWS:
      {
        // Waiting for the timer trigger to update PWM duty cycles
        if (m_systemFlags->updatePwmDutyCycles.exchange(false))
        {
          m_debugPin.pulse();
          // printToConsole("4\r\n");
          m_controlState = ControlState::CONTROL_UPDATE_PWM_DUTY_CYCLES;
        }
        break;
      }
      case ControlState::CONTROL_UPDATE_PWM_DUTY_CYCLES:
      {
        m_debugPin.pulse();
        // printToConsole("5\r\n");
        m_controlState = ControlState::CONTROL_FINISHED_CYCLE;
        break;
      }
      case ControlState::CONTROL_FINISHED_CYCLE:
      {
        // Done updating, cycle completed
        // Waiting for timer trigger to start next cycle
        if (m_systemFlags->startControlCycle.exchange(false))
        {
          m_debugPin.pulse();
          // printToConsole("6\r\n");
          m_controlState = ControlState::CONTROL_START_SAMPLING;
        }
        break;
      }
    }
  }

  bool checkForError()
  {
    return m_errorPin.isOn();
  }
};

void Tests::stateMachines(SystemFlags* systemFlags, ClutchCurrentSampler* clutch1, ClutchCurrentSampler* clutch2, ClutchCurrentSampler* clutch3, MotorVelocitySampler* motor)
{
  TestStateMachines testStateMachines(systemFlags, clutch1, clutch2, clutch3, motor);
  testStateMachines.init();

  while (true)
  {
    testStateMachines.execute();
  }
}
