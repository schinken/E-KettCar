#include <Arduino.h>
#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Motor.h"
#include "GasPedal.h"
#include "ExponentialSmoothing.h"
#include "settings.h"


uint8_t newDirection = DIRECTION_NONE;
float batteryVoltage = 0.0;

Bounce switchForward = Bounce();
Bounce switchBackward = Bounce();

Motor motor = Motor(PIN_ENABLE_R, PIN_PWM_R, PIN_ENABLE_L, PIN_PWM_L);
GasPedal gas = GasPedal(PIN_GAS_PEDAL, GAS_VALUE_MIN, GAS_VALUE_MAX);

ExponentialSmoothing smoothGas = ExponentialSmoothing(ExponentialSmoothing::DEFAULT_ALPHA);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  
  Wire.begin();
  Serial.begin(115200);

  lcd.begin();
  lcd.clear();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  
  pinMode(PIN_BATTERY_VOLTAGE, INPUT);
  
  pinMode(PIN_SWITCH_FORWARD, INPUT_PULLUP);
  switchForward.attach(PIN_SWITCH_FORWARD);
  switchForward.interval(100);

  pinMode(PIN_SWITCH_BACKWARD, INPUT_PULLUP);
  switchBackward.attach(PIN_SWITCH_BACKWARD);
  switchBackward.interval(100);
}


float getBatteryVoltage() {
  float reading = analogRead(PIN_BATTERY_VOLTAGE);
  Serial.print("RawBat: ");
  Serial.println(reading);
  return mapfloat(reading, BATTERY_READING_6V, BATTERY_READING_12V, 6.0, 12.0);
}

void updateDisplay() {
  lcd.setCursor(0, 0);
  lcd.print("Geschw: ");
  lcd.print(map(motor.getSpeed(), 0, 255, 0, 100));
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("Bat: ");
  lcd.print(batteryVoltage);
  lcd.print("V   ");
}

void loop() {
  uint8_t speed = 0;
  uint8_t currentGas = gas.getValue();
  uint8_t currentBatteryVoltage = getBatteryVoltage();
  
  smoothGas.update(currentGas);

  // Different max speed, when driving backwards
  if (motor.isForwards()) {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_FORWARD);
  } else {
    speed = map(smoothGas.getValue(), 0, 255, 0, SPEED_MAX_BACKWARD);
  }

  Serial.print("Speed: ");
  Serial.println(speed);

  motor.setSpeed(speed);


/*
  TODO: Battery Protection
  batteryVoltage = getBatteryVoltage();
  while (batteryVoltage > 3 && batteryVoltage < 11.4) {
    changeSpeed(currentDirection, speed, 0);
    batteryVoltage = getBatteryVoltage();

    delay(1000);
  }
  */


  // Remember to switch forwards
  if (switchForward.fell() && !motor.isForwards()) {
    newDirection = Motor::DIRECTION_FORWARD;
    Serial.println(">>>>>>>>>>> Forward!");
  }

  // Remember to switch backwards
  if (switchBackward.fell() && !motor.isBackwards()) {
    newDirection = Motor::DIRECTION_BACKWARD;
    Serial.println(">>>>>>>>>> Backward!");
  }


  // Switch direction if speed is low enough
  if (newDirection && speed < SPEED_MIN_DIRECTION_CHANGE) {

    motor.changeSpeed(0, 10);
    motor.setDirection(newDirection);
    motor.changeSpeed(speed, 10);
    
    newDirection = DIRECTION_NONE;
  }

  
  switchForward.update();
  switchBackward.update();

  updateDisplay();
}
