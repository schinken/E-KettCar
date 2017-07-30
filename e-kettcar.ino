#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Motor.h"
#include "Battery.h"
#include "GasPedal.h"
#include "Gear.h"
#include "ExponentialSmoothing.h"
#include "settings.h"

Motor motor = Motor(PIN_ENABLE_R, PIN_PWM_R, PIN_ENABLE_L, PIN_PWM_L);
Gear gear = Gear(PIN_SWITCH_FORWARD, PIN_SWITCH_BACKWARD);
GasPedal gas = GasPedal(PIN_GAS_PEDAL, GAS_VALUE_MIN, GAS_VALUE_MAX);
Battery battery = Battery(PIN_BATTERY_VOLTAGE);

ExponentialSmoothing smoothGas = ExponentialSmoothing();
ExponentialSmoothing smoothBattery = ExponentialSmoothing();

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  
  Wire.begin();
  Serial.begin(115200);

  lcd.begin();
  lcd.clear();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
}

void updateDisplay() {
  lcd.setCursor(0, 0);
  lcd.print("Geschw: ");
  lcd.print(map(motor.getSpeed(), 0, 255, 0, 100));
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("Bat: ");
  lcd.print(smoothBattery.getValue());
  lcd.print("V   ");
}

void loop() {
  uint8_t speed = 0;
  uint8_t currentGas = gas.getValue();
  uint8_t currentBatteryVoltage = battery.getValue();
  
  smoothGas.update(currentGas);
  smoothBattery.update(currentBatteryVoltage);

  // Different max speed, when driving backwards
  if (motor.isForwards()) {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_FORWARD);
  } else {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_BACKWARD);
  }

  Serial.print("Speed: ");
  Serial.println(speed);

  motor.setSpeed(speed);


  /* TODO: Battery Protection
  batteryVoltage = getBatteryVoltage();
  while (smoothBattery.getValue() > 3 && smoothBattery.getValue() < 11.4) {
    changeSpeed(currentDirection, speed, 0);
    motor.changeSpeed(0);
    delay(1000);
  }
  */

  // Does the gear doesn't match with the motor gear?
  if ((gear.isForwards() && !motor.isForwards()) ||
      (gear.isBackwards() && !motor.isBackwards())) {

    // Change gear only, if speed is below a configured speed
    if (speed < SPEED_MIN_DIRECTION_CHANGE) {
      motor.changeSpeed(0, 10);
      motor.setDirection(gear.getGear());
      motor.changeSpeed(speed, 10);
    }
   
  }

  gear.update();
  updateDisplay();
}
