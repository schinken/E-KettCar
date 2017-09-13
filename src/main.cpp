#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Motor.h>
#include <Battery.h>
#include <BatteryProtection.h>
#include <GasPedal.h>
#include <Gear.h>
#include <ExponentialSmoothing.h>
#include <SimpleTimer.h>
#include <Display.h>
#include "settings.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

Motor motor(PIN_CW, PIN_CCW);
Gear gear(PIN_SWITCH_FORWARDS, PIN_SWITCH_BACKWARDS);
GasPedal gas(PIN_GAS_PEDAL, GAS_VALUE_MIN, GAS_VALUE_MAX);
Battery battery(PIN_BATTERY_VOLTAGE, BATTERY_READING_6V, BATTERY_READING_12V);

BatteryProtection batteryProtection(&battery);
Display display(&lcd);

ExponentialSmoothing smoothGas(0.7);
ExponentialSmoothing smoothBattery;

bool toggle = false;
SimpleTimer timer;

void toggleInterval() {
  toggle = !toggle;
}

void updateDisplayInterval() {
  if (batteryProtection.isLowPower()) {
    if (toggle) {
      display.lowBattery();
    } else {
      display.normalOperation(&gear, smoothGas.getValue(), smoothBattery.getValue());
    }

    return;
  }

  display.normalOperation(&gear, smoothGas.getValue(), smoothBattery.getValue());
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  gas.begin();
  gear.begin();
  motor.begin();
  battery.begin();
  display.begin();

  display.welcome();
  delay(1300);

  smoothBattery.setValue(battery.getValue());

  timer.setInterval(2000, toggleInterval);
  timer.setInterval(400,  updateDisplayInterval);
}

void loop() {

  uint8_t speed = 0;
  uint8_t currentGas = gas.getValue();
  float currentBatteryVoltage = battery.getValue();

  while(batteryProtection.isBatteryProtection()) {
    motor.setSpeed(0);
    display.batteryProtection();

    batteryProtection.update();
    delay(500);
  }

  // Different max speed, when driving backwards
  if (batteryProtection.isLowPower()) {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_BATTERY_LOW_POWER);
  } else if (motor.isForwards()) {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_FORWARDS);
  } else {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_BACKWARDS);
  }

  motor.setSpeed(speed);

  Serial.print("B1: ");
  Serial.println(smoothBattery.getValue());

  // Does the gear doesn't match with the motor gear?
  if ((gear.isForwards() && !motor.isForwards()) ||
      (gear.isBackwards() && !motor.isBackwards())) {

    // Change gear only, if speed is below a configured speed
    if (speed < SPEED_MAX_DIRECTION_CHANGE) {
      motor.changeSpeed(0, SPEED_CHANGE_PACE_DEFAULT);
      motor.setDirection(gear.getGear());
      motor.changeSpeed(speed, SPEED_CHANGE_PACE_DEFAULT);
    }
  }

  gear.update();
  smoothGas.update(currentGas);
  smoothBattery.update(currentBatteryVoltage);
  batteryProtection.update();

  timer.run();
}
