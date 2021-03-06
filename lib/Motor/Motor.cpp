#include "Motor.h"

Motor::Motor(uint8_t pinCW, uint8_t pinCCW)
  : pinCW(pinCW), pinCCW(pinCCW),
    speed(0), direction(Motor::DIRECTION_FORWARDS) {
}

void Motor::begin() {
  pinMode(this->pinCW, OUTPUT);
  digitalWrite(this->pinCW, LOW);

  pinMode(this->pinCCW, OUTPUT);
  digitalWrite(this->pinCCW, LOW);
}

void Motor::setSpeed(uint8_t speed) {

  uint8_t active = this->pinCCW;
  uint8_t inactive = this->pinCW;

  if (this->direction == Motor::DIRECTION_FORWARDS) {
    active = this->pinCW;
    inactive = this->pinCCW;
  }

  // Make sure there's absolutely no signal on there
  // opposite direction pin
  digitalWrite(inactive, LOW);

  if (speed < 3) {
    digitalWrite(active, LOW);
  } else if (speed > 250) {
    digitalWrite(active, HIGH);
  } else {
    analogWrite(active, speed);
  }

  this->speed = speed;
}

void Motor::setDirection(uint8_t direction) {
  // Speed totally off.
  digitalWrite(this->pinCCW, LOW);
  digitalWrite(this->pinCW, LOW);

  this->direction = direction;
}

void Motor::slowdown(uint8_t speed, uint8_t pace) {
  for (uint8_t i = this->getSpeed(); i > speed; i--) {
    this->setSpeed(i);
    delay(pace);
  }
}

void Motor::speedup(uint8_t speed, uint8_t pace) {
  for (uint8_t i = this->getSpeed(); i < speed; i++) {
    this->setSpeed(i);
    delay(10);
  }
}

void Motor::changeSpeed(uint8_t speed, uint8_t pace) {
  if (this->getSpeed() > speed) {
    this->slowdown(speed, pace);
  } else {
    this->speedup(speed, pace);
  }
}

void Motor::setForward() {
  this->setDirection(Motor::DIRECTION_FORWARDS);
}

void Motor::setBackward() {
  this->setDirection(Motor::DIRECTION_BACKWARDS);
}

bool Motor::isForwards() {
  return (this->direction == Motor::DIRECTION_FORWARDS);
}

bool Motor::isBackwards() {
  return (this->direction == Motor::DIRECTION_BACKWARDS);
}

uint8_t Motor::getSpeed() {
  return this->speed;
}
