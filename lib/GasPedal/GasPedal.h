#ifndef __GASPEDAL_H__
#define __GASPEDAL_H__

#include <Arduino.h>

class GasPedal {

    public:
      GasPedal(uint8_t pin);
      GasPedal(uint8_t pin, uint16_t minValue, uint16_t maxValue);

      void begin();
      
      uint8_t getValue();
      uint16_t getRawValue();

      void setMinimum(uint16_t value);
      void setMaximum(uint16_t value);

    private:

      uint8_t pin;
      uint8_t gas;

      uint16_t minValue;
      uint16_t maxValue;
};

#endif
