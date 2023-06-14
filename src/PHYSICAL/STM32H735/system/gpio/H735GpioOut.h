#ifndef _H735_GPIO_OUT_H_
#define _H735_GPIO_OUT_H_

#include "H735Gpio.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735GpioOut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735GpioOut : public H735Gpio
{
public:
  H735GpioOut (TypeGpioBank, int pin_number);
  ~H735GpioOut ();

  void activatePushPull (TypeGpioSpeed, bool b_value = false);
  void activateOpenDrain (TypeGpioSpeed, TypeGpioLoad, bool b_value = false);
  void setOutput (bool b_value);

private:
  void activate (bool b_value, TypeGpioSpeed, TypeGpioLoad, TypeGpioOutput);
};

typedef H735GpioOut PhysicalGpioOut;

#endif  // _H735_GPIO_OUT_H_
