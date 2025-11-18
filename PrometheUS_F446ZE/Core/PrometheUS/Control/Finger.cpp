#include "Finger.hpp"

Finger::Finger(ClutchCurrentSampler* clutchCurrentSampler)
  : m_clutch(clutchCurrentSampler)
{

}

void Finger::init(
    uint32_t      clutchCurrentChannel,
    uint8_t       clutchCurrentIndex,
    GPIO_TypeDef* encoderChipSelectPeripheral,
    uint16_t      encoderChipSelectPin,
    uint32_t      pwmChannel,
    GPIO_TypeDef* portPWMn,
    uint16_t      pinPWMn)
{
  m_clutch.init(clutchCurrentChannel, clutchCurrentIndex, pwmChannel, portPWMn, pinPWMn);
  m_encoder.init(&hspi1, encoderChipSelectPeripheral, encoderChipSelectPin);
}

void Finger::sampleEncoder()
{
  m_encoder.sampleRawValue();
}

float Finger::getClutchMeasuredCurrent()
{
  return m_clutch.getMeasuredCurrent();
}

bool Finger::isClutchError()
{
  return m_clutch.isInError();
}

float Finger::getEncoderPosition()
{
  return m_encoder.convertValue();
}

void Finger::calculateCommand(float potentiometerPosition)
{
  // High level control law here
  // Input is potentiometer position
  // Output is clutch current
  float targetCurrent = (potentiometerPosition * MAX_CLUTCH_CURRENT_AMPERE) / MAX_POTENTIOMETER_POSITION_DEG;
  float measuredCurrent = m_clutch.getMeasuredCurrent();

  m_clutch.calculateCurrentPID(targetCurrent, measuredCurrent);
}

void Finger::updateCommand()
{
  m_clutch.updateCommand();
}

void Finger::start()
{
  m_clutch.start();
}

void Finger::stop()
{
  m_clutch.stop();
}
