#include <Display.h>

Display::Display(LiquidCrystal_I2C* lcd) {
  this->lcd = lcd;
}

void Display::lowBattery() {
  this->lcd->setCursor(0, 0);
  this->lcd->print(" Batterie leer! ");

  this->lcd->setCursor(0, 1);
  this->lcd->print("-Bitte aufladen-");
}

void Display::batteryProtection() {
  this->lcd->setCursor(0, 0);
  this->lcd->print(" Tiefendladungs-");

  this->lcd->setCursor(0, 1);
  this->lcd->print(" Schutz!        ");
}

void Display::welcome() {
  this->lcd->setCursor(3, 0);
  this->lcd->print("Tim Huber");

  this->lcd->setCursor(0, 1);
  this->lcd->print("Model: E-Kettcar");
}

void Display::normalOperation(Gear *gear, uint8_t gas, float battery) {
  this->lcd->setCursor(0, 0);
  this->lcd->print("Geschw: ");
  this->lcd->print(map(gas, 0, 255, 0, 100));
  this->lcd->print("%       ");

  this->lcd->setCursor(15, 0);
  if (gear->isForwards()) {
    this->lcd->print("V");
  } else {
    this->lcd->print("R");
  }

  this->lcd->setCursor(0, 1);
  this->lcd->print("Bat: ");
  this->lcd->print(battery);
  this->lcd->print("V          ");
}


void Display::begin() {
  this->lcd->init();
  this->lcd->clear();
  this->lcd->backlight();

  this->lcd->setCursor(0, 0);
}
