#pragma once

#include "Clutch.hpp"
#include "EncoderSampler.hpp"

class Finger
{
public:
  Finger(ClutchCurrentSampler* clutchCurrentSampler);
  ~Finger() = default;

  void init(
      uint32_t      clutchCurrentChannel,
      uint8_t       clutchCurrentIndex,
      GPIO_TypeDef* encoderChipSelectPeripheral,
      uint16_t      encoderChipSelectPin,
      uint32_t      pwmChannel);

  // Read encoder
  void sampleEncoder();

  // Clutch helper functions
  float getClutchMeasuredCurrent();
  bool isClutchError();

  float getEncoderPosition();

  // Calculate high-level command
  void calculateCommand(float potentiometerPosition);

  // Update clutch PWM
  void updateCommand();

  void start();
  void stop();

private:
  Clutch          m_clutch;
  EncoderSampler  m_encoder;
};
