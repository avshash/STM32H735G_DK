#ifndef _I2C_H_
#define _I2C_H_

#include "TypeI2cConfigurationParam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2c
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class I2c
{
public:
  virtual void activateGpio () = 0;
  virtual void configure (const uint32_t params[I2C_CONFIGURATION_PARAMS_COUNT]) = 0;
};

#endif  // _I2C_H_