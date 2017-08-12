#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <Arduino.h>

class Motor {

  public:
    Motor(uint8_t pinCW, uint8_t pinCCW);

    static const uint8_t DIRECTION_FORWARDS = 1;
    static const uint8_t DIRECTION_BACKWARDS = 2;

    void begin();
    void setSpeed(uint8_t speed);
    void setDirection(uint8_t direction);
    void setForward();
    void setBackward();

    void slowdown(uint8_t speed, uint8_t pace);
    void speedup(uint8_t speed, uint8_t pace);
    void changeSpeed(uint8_t speed, uint8_t pace);

    bool isForwards();
    bool isBackwards();

    uint8_t getSpeed();

  private:
    uint8_t pinCW;
    uint8_t pinCCW;

    uint8_t speed;
    uint8_t direction;
};

#endif
