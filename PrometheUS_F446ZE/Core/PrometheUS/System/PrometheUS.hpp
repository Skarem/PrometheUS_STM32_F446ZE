#pragma once

#include "DigitalOutput.hpp"
#include "DigitalInput.hpp"

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
      SystemFlags &systemFlags,
      ClutchCurrentSampler &clutchCurrentSampler1,
      ClutchCurrentSampler &clutchCurrentSampler2,
      ClutchCurrentSampler &clutchCurrentSampler3,
      MotorVelocitySampler &motorVelocitySampler);

  void init();

  void execute();

  static void startTimers();

private:

  // Data sender
  TelemetrySender m_telemetrySender;
  uint8_t         m_sendDataCounter = 0;

  const uint8_t SEND_DATA_TIMING = 200; // 1000 Hz / 50 Hz

  // System flags
  SystemFlags &m_systemFlags;
  // Error flags
  ErrorFlags m_errorFlags;

  // LED (RUN state)
  DigitalOutput m_led1;
  // LED (ERROR state)
  DigitalOutput m_led2;

  // Switch (Toggle between IDLE and RUN states)
  DigitalInput m_switch;

  // Potentiometers
  PotentiometersSampler m_potentiometers;
  float m_potentiometerPositionsArray[FINGER_COUNT] = {0};

  // Clutch temperatures
  ClutchesTemperatureSampler m_clutchesTemperature;
  float m_clutchTemperaturesArray[FINGER_COUNT] = {0};

  float m_clutchCurrentsArray[FINGER_COUNT] = {0};
  float m_encoderPositions[FINGER_COUNT]    = {0};

  const float MIN_CLUTCH_TEMPERATURE = 0.0f;
  const float MAX_CLUTCH_TEMPERATURE = 75.0f;

  // Fingers
  Finger m_finger1;
  Finger m_finger2;
  Finger m_finger3;

  // Motor (ESCON 5/50 Drive)
  Motor m_motor;

  ControlState  m_controlState  = ControlState::CONTROL_FINISHED_CYCLE;
  SystemState   m_systemState   = SystemState::SYS_INIT;

  void systemStateMachine();
  void controlStateMachine();

  void start();
  void stop();

  void checkForErrors();
};





