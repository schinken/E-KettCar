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

  if (this->direction == Motor::DIRECTION_FORWARDS) {
    digitalWrite(this->pinCCW, LOW);

    if (speed < 3) {
      digitalWrite(this->pinCW, LOW);
    } else {
      analogWrite(this->pinCW, speed);
    }
  } else {
    digitalWrite(this->pinCW, LOW);

    if (speed < 3) {
      digitalWrite(this->pinCCW, LOW);
    } else {
      analogWrite(this->pinCCW, speed);
    }
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
