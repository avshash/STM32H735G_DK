#ifndef _I2C_H_
#define _I2C_H_

#include "TypeI2cConfigurationParam.h"
#include "Interrupt.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2c
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class I2c : protected Interrupt
{
public:
  virtual void activateGpio () = 0;
  virtual void configure (uint32_t frequency) = 0;
};

#endif  // _I2C_H_
