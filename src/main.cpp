#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Motor.h>
#include <Battery.h>
#include <GasPedal.h>
#include <Gear.h>
#include <ExponentialSmoothing.h>
#include <SimpleTimer.h>
#include "settings.h"

Motor motor(PIN_CW, PIN_CCW);
Gear gear(PIN_SWITCH_FORWARDS, PIN_SWITCH_BACKWARDS);
GasPedal gas(PIN_GAS_PEDAL, GAS_VALUE_MIN, GAS_VALUE_MAX);
Battery battery(PIN_BATTERY_VOLTAGE, BATTERY_READING_6V, BATTERY_READING_12V);

ExponentialSmoothing smoothGas(0.7);
ExponentialSmoothing smoothBattery;
ExponentialSmoothing ultraSmoothBattery(0.03);

bool toggle = false;
bool isBatteryProtectionMode = false;

SimpleTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void displayLowBattery() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Batterie leer! ");

  lcd.setCursor(0, 1);
  lcd.print("-Bitte aufladen-");
}

void displayVeryLowBattery() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Tiefendladungs-");

  lcd.setCursor(0, 1);
  lcd.print(" Schutz!        ");
}

void displayWelcome() {
  lcd.setCursor(3, 0);
  lcd.print("Tim Huber");

  lcd.setCursor(0, 1);
  lcd.print("Model: E-Kettcar");
}

void displayOperational() {
  lcd.clear();

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

void toggleInterval() {
  toggle = !toggle;
}

void updateDisplayInterval() {
  if (isBatteryProtectionMode) {
    if (toggle) {
      displayLowBattery();
    } else {
      displayOperational();
    }

    return;
  }

  displayOperational();
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
  ultraSmoothBattery.setValue(battery.getValue());

  displayWelcome();
  delay(1300);
  lcd.clear();

  timer.setInterval(1000, toggleInterval);
  timer.setInterval(100,  updateDisplayInterval);
}

void loop() {

  uint8_t speed = 0;
  uint8_t currentGas = gas.getValue();
  float currentBatteryVoltage = battery.getValue();

  // Different max speed, when driving backwards
  if (isBatteryProtectionMode) {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_BATTERY_PROTECTION);
  } else if (motor.isForwards()) {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_FORWARDS);
  } else {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_BACKWARDS);
  }

  Serial.print("S: ");
  Serial.println(speed);

  motor.setSpeed(speed);

  Serial.print("B: ");
  Serial.println(smoothBattery.getValue());

  // Battery protection!
  if (ultraSmoothBattery.getValue() < 11.6) {
    isBatteryProtectionMode = true;
  } else if (ultraSmoothBattery.getValue() > 13.0) {
    isBatteryProtectionMode = false;
  } else {

    // Now we really, really need to protect this battery!
    while (ultraSmoothBattery.getValue() < 10.6) {
      motor.setSpeed(0);
      displayVeryLowBattery();
      delay(5000);
    }
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
  ultraSmoothBattery.update(currentBatteryVoltage);

  timer.run();
}
