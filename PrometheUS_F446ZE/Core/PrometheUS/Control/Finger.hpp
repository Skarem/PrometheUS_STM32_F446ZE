#pragma once

#include "Clutch.hpp"
#include "EncoderSampler.hpp"

class Finger
{
public:
  Finger(ClutchCurrentSampler &clutchCurrentSampler);
  ~Finger() = default;

  void init(
      uint32_t      clutchCurrentChannel,
      uint8_t       clutchCurrentIndex,
      GPIO_TypeDef* encoderChipSelectPeripheral,
      uint16_t      encoderChipSelectPin,
      uint32_t      pwmChannel);

  // Read encoder
  void sampleEncoder();

  void calculateCommand(float potentiometerPosition, float clutchTemperature);

  void updateCommand();

  void stop();

private:
  Clutch          m_clutch;
  EncoderSampler  m_encoder;
};
