#ifndef __BATTERY_H__
#define __BATTERY_H__

#include <Arduino.h>

class Battery {

    public:
      Battery(uint8_t pin);

      float getValue();
      uint16_t getRawValue();

      void setCalibrationValue6V(uint16_t value);
      void setCalibrationValue12V(uint16_t value);
      
    private:

      uint8_t pin;

      uint16_t calibration6V;
      uint16_t calibration12V;

      float mapfloat(long x, long in_min, long in_max, long out_min, long out_max);
};

#endif
