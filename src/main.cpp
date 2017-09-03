#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Motor.h>
#include <Battery.h>
#include <GasPedal.h>
#include <Gear.h>
#include <ExponentialSmoothing.h>
#include "settings.h"

Motor motor(PIN_CW, PIN_CCW);
Gear gear(PIN_SWITCH_FORWARDS, PIN_SWITCH_BACKWARDS);
GasPedal gas(PIN_GAS_PEDAL, GAS_VALUE_MIN, GAS_VALUE_MAX);
Battery battery(PIN_BATTERY_VOLTAGE, BATTERY_READING_6V, BATTERY_READING_12V);

ExponentialSmoothing smoothGas(0.7);
ExponentialSmoothing smoothBattery;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void displayLowBattery() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Batterie leer!");
}

void displayWelcome() {
  lcd.setCursor(3, 0);
  lcd.print("Tim Huber");
  lcd.setCursor(0, 1);
  lcd.print("Model: E-Kettcar");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  gas.begin();
  gear.begin();
  motor.begin();
  battery.begin();

  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0, 0);

  smoothBattery.setValue(battery.getValue());

  displayWelcome();
  delay(3000);
  lcd.clear();
}

void displayOperational() {
  lcd.setCursor(0, 0);
  lcd.print("Geschw: ");
  lcd.print(map(smoothGas.getValue(), 0, 255, 0, 100));
  lcd.print("%   ");

  lcd.setCursor(15, 0);
  if (gear.isForwards()) {
    lcd.print("V");
  } else {
    lcd.print("R");
  }

  lcd.setCursor(0, 1);
  lcd.print("Bat: ");
  lcd.print(smoothBattery.getValue());
  lcd.print("V   ");
}

void loop() {

  uint8_t speed = 0;
  uint8_t currentGas = gas.getValue();
  float currentBatteryVoltage = battery.getValue();

  // Different max speed, when driving backwards
  if (motor.isForwards()) {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_FORWARDS);
  } else {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_BACKWARDS);
  }

  Serial.print("S: ");
  Serial.println(speed);

  motor.setSpeed(speed);

  Serial.print("B: ");
  Serial.println(smoothBattery.getValue());


  while (smoothBattery.getValue() > 3 && smoothBattery.getValue() < 11.2) {
    motor.setSpeed(0);
    displayLowBattery();
    delay(4000);
  }

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

  displayOperational();
}
