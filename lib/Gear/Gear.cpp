#include "Gear.h"
#include "Motor.h"

Gear::Gear(uint8_t pinForwards, uint8_t pinBackwards)
  : gear(Motor::DIRECTION_FORWARDS) {
    
  pinMode(pinForwards, INPUT_PULLUP);
  pinMode(pinBackwards, INPUT_PULLUP);

  this->debounceForwards = new Bounce();
  this->debounceForwards->attach(pinForwards);
  this->debounceForwards->interval(100);

  this->debounceBackwards = new Bounce();
  this->debounceBackwards->attach(pinBackwards);
  this->debounceBackwards->interval(100);
}

void Gear::update() {
  this->debounceForwards->update();
  this->debounceBackwards->update();

  if (this->debounceForwards->fell()) {
    this->gear = Motor::DIRECTION_FORWARDS;
  }

  if (this->debounceBackwards->fell()) {
    this->gear = Motor::DIRECTION_BACKWARDS;
  }

}

bool Gear::isForwards() {
  return (this->gear == Motor::DIRECTION_FORWARDS);
}

bool Gear::isBackwards() {
  return (this->gear == Motor::DIRECTION_BACKWARDS);
}

uint8_t Gear::getGear() {
  return this->gear;
}
