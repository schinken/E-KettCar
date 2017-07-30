#define PIN_GAS_PEDAL A0
#define PIN_BATTERY_VOLTAGE A1

#define BATTERY_READING_12V 678
#define BATTERY_READING_6V 337

#define BATTERY_VOLTAGE_MIN 11

#define PIN_ENABLE_R 4
#define PIN_ENABLE_L 7

#define PIN_PWM_R 10
#define PIN_PWM_L 9

#define PIN_SWITCH_FORWARDS 12
#define PIN_SWITCH_BACKWARDS 13

#define GAS_VALUE_MIN 436
#define GAS_VALUE_MAX 555

#define SPEED_MIN 0
#define SPEED_MIN_DIRECTION_CHANGE 15
#define SPEED_MAX_FORWARDS 255
#define SPEED_MAX_BACKWARDS 255

#define EXPO_SMOOTHING_ALPA 0.3


float mapfloat(long x, long in_min, long in_max, long out_min, long out_max) {
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
