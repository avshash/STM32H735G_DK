#ifndef _H735_GPIO_AF_H_
#define _H735_GPIO_AF_H_

#include "H735Gpio.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735GpioAf
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735GpioAf : public H735Gpio
{
public:
  H735GpioAf (TypeGpioBank, uint32_t pin_number);
  ~H735GpioAf ();

  void activate (int af_index, TypeGpioSpeed, TypeGpioLoad, TypeGpioOutput);
};

typedef H735GpioAf PhysicalGpioAf;

#endif  // _H735_GPIO_AF_H_
