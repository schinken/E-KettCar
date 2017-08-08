#ifndef __EXPONENTIAL_SMOOTHING_H__
#define __EXPONENTIAL_SMOOTHING_H__

#include <Arduino.h>

class ExponentialSmoothing {

    public:
      ExponentialSmoothing();
      ExponentialSmoothing(float alpha);
      ExponentialSmoothing(float initialValue, float alpha);

      const float DEFAULT_ALPHA = 0.03;

      float getValue();

      void update(float newValue);

    private:
      float smoothValue;
      float alpha;
};

#endif
