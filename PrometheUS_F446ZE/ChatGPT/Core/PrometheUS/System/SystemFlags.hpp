#pragma once

#include <atomic>

class SystemFlags
{
public:
  static SystemFlags* instance;

  SystemFlags()
  {
    instance = this;
  }

  // ===== Control loop / timer flags =====
  std::atomic<bool> startControlCycle   { false };
  std::atomic<bool> updatePwmDutyCycles { false };
  std::atomic<bool> errorDetected       { false };

  // ===== ADC sampling flags =====
   std::atomic<bool> motorVelocityDone  { false };

   static constexpr uint8_t POTS  = 1u << 0;
   static constexpr uint8_t TEMP  = 1u << 1;
   static constexpr uint8_t CURR  = 1u << 2;
   static constexpr uint8_t ALL   = POTS | TEMP | CURR;

   std::atomic<uint8_t> adcDoneMask { 0 };

   enum class ADCSource : uint8_t
   {
     NONE     = 0,
     CLUTCH_1 = 1,
     CLUTCH_2 = 2,
     CLUTCH_3 = 3,
     MOTOR    = 4
   };

   std::atomic<ADCSource> lastActiveADC { ADCSource::NONE };
};
