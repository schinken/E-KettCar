#include "Motor.h"

Motor::Motor(uint8_t enableR, uint8_t pwmR, uint8_t enableL, uint8_t pwmL)
  : enableR(enableR), pwmR(pwmR),
    enableL(enableL), pwmL(pwmL),
    speed(0), direction(Motor::DIRECTION_FORWARDS) {

  pinMode(enableR, OUTPUT);
  digitalWrite(enableR, LOW);

  pinMode(pwmR, OUTPUT);
  analogWrite(pwmR, 0);

  pinMode(enableL, OUTPUT);
  digitalWrite(enableL, LOW);

  pinMode(pwmL, OUTPUT);
  analogWrite(pwmL, 0);
}

void Motor::setSpeed(uint8_t speed) {

  if (direction == Motor::DIRECTION_FORWARDS) {
    analogWrite(pwmL, 0);
    analogWrite(pwmR, speed);
  } else {
    analogWrite(pwmR, 0);
    analogWrite(pwmL, speed);
  }

  this->speed = speed;
}

void Motor::setDirection(uint8_t direction) {
  analogWrite(pwmL, 0);
  analogWrite(pwmR, 0);

  if (direction == Motor::DIRECTION_FORWARDS) {
    digitalWrite(enableL, LOW);
    digitalWrite(enableR, HIGH);
  } else {
    digitalWrite(enableR, LOW);
    digitalWrite(enableL, HIGH);
  }

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
