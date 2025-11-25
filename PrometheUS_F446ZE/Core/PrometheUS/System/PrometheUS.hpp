#pragma once

#include "DigitalOutput.hpp"
#include "DebouncedButton.hpp"
#include "LED.hpp"

#include "PotentiometersSampler.hpp"
#include "ClutchesTemperatureSampler.hpp"

#include "Finger.hpp"
#include "ClutchCurrentSampler.hpp"

#include "Motor.hpp"
#include "MotorVelocitySampler.hpp"

#include "StateMachines.hpp"
#include "SystemFlags.hpp"
#include "ErrorFlags.hpp"

#include "TelemetrySender.hpp"

class PrometheUS_Gripper
{
public:

  PrometheUS_Gripper(
      SystemFlags* systemFlags,
      ClutchCurrentSampler* clutchCurrentSampler1,
      ClutchCurrentSampler* clutchCurrentSampler2,
      ClutchCurrentSampler* clutchCurrentSampler3,
      MotorVelocitySampler* motorVelocitySampler);

  void init();

  void execute();

  static void startTimers();

private:

  const bool verbose    = false;
  const bool visualize  = true;

  void printConsole(const char message[], uint16_t messageLength);

  DigitalOutput m_debugPin;

  // Data sender
  TelemetrySender m_telemetrySender;
  uint8_t         m_sendDataCounter = 0;

  const uint8_t SEND_DATA_TIMING = 100; // 100 cycles at 1KHz => 10 Hz telemetry

  // System flags
  SystemFlags* m_systemFlags;

  // Error flags
  ErrorFlags m_errorFlags;
  // Error source
  ErrorSource m_errorSource = ErrorSource::NONE;
  bool m_sendErrorMessage = true;

  // LED (RUN state)
  LED m_ledRun;
  // LED (ERROR state)
  LED m_ledError;

  // Button (Toggle between IDLE and RUN states)
  DebouncedButton m_button;

  // Potentiometers
  PotentiometersSampler m_potentiometers;
  float m_potentiometerPercentagesArray[FINGER_COUNT] = {0};

  // Clutch temperatures
  ClutchesTemperatureSampler m_clutchesTemperature;
  float m_clutchTemperaturesArray[FINGER_COUNT] = {0};

  float m_clutchDutyCyclesArray[FINGER_COUNT] = {0};

  float m_motorVelocity = 0;

  // Fingers
  Finger m_finger1;
  Finger m_finger2;
  Finger m_finger3;

  // Motor
  Motor m_motor;

  const float BASE_MOTOR_VELOCITY = 150.0f;
  const float RPM_INCREASE_PER_FINGER_MOVING = 100.0f;
  float m_oldTargetRPM = BASE_MOTOR_VELOCITY;

  // High-level state machine's state
  SystemState   m_systemState   = SystemState::SYS_INIT;

  // Low-level state machine's state
  ControlState  m_controlState  = ControlState::CONTROL_FINISHED_CYCLE;

  void systemStateMachine();

  void doIdle();
  void doRun();
  void doError();

  void controlStateMachine();

  void doStartSampling();
  void doWaitSampling();
  void doCalculate();
  void doFinishedControlLaws();
  void doUpdatePWM();
  void doFinishedCycle();

  void readSensors();
  void maybeSendTelemetry();
  void computeErrors();
  void computeControlLaws();

  void mapPotentiometersToDutyCycleWithDeadZones();

  void start();
  void stop();

  uint8_t m_tempErrorCounters[FINGER_COUNT] = {0};
  static constexpr uint8_t TEMP_ERROR_COUNT_THRESHOLD = 5;
  void checkForErrors(bool useFiltering);
  inline void setErrorSource(size_t index);
};





