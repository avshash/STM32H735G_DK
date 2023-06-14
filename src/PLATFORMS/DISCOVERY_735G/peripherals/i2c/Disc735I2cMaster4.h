#ifndef _DISC735_I2C_MASTER_4_H_
#define _DISC735_I2C_MASTER_4_H_

#include "PhysicalI2cMaster.h"
#include "PhysicalGpioAf.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735I2cMaster4
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735I2cMaster4 : public PhysicalI2cMaster
{
public:
  Disc735I2cMaster4 ();

  virtual void activateGpio () final;

private:
  PhysicalGpioAf m_scl;
  PhysicalGpioAf m_sda;
};

typedef Disc735I2cMaster4 PlatformI2cMaster4;

#endif  // _DISC735_I2C_MASTER_4_H_

