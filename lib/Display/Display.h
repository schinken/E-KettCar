#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Arduino.h>
#include <Gear.h>
#include <LiquidCrystal_I2C.h>

class Display {

    public:
      Display(LiquidCrystal_I2C* lcd);

      void begin();
      void update();

      void lowBattery();
      void batteryProtection();
      void welcome();
      void normalOperation(Gear *gear, uint8_t gas, float battery);

    private:
      LiquidCrystal_I2C* lcd;
};

#endif
