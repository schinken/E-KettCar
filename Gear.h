#ifndef __GEAR_H__
#define __GEAR_H__

#include <Arduino.h>
#include <Bounce2.h>

class Gear {

    public:
      Gear(uint8_t pinForwards, uint8_t pinBackwards);

      void update();
      
      bool isForwards();
      bool isBackwards();

      uint8_t getGear();
      
    private:

      Bounce* debounceForwards;
      Bounce* debounceBackwards;

      uint8_t gear;
};

#endif
