#ifndef _DISC735_I2C_MASTER_MANAGER_4_H_
#define _DISC735_I2C_MASTER_MANAGER_4_H_

#include "PhysicalI2cMasterManager.h"
#include "PhysicalGpioAf.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735I2cMasterManager4
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735I2cMasterManager4 : public PhysicalI2cMasterManager
{
public:
  Disc735I2cMasterManager4 ();

  virtual void activateGpio () final;

private:
  PhysicalGpioAf m_scl;
  PhysicalGpioAf m_sda;
};

typedef Disc735I2cMasterManager4 PlatformI2cMasterManager4;

#endif  // _DISC735_I2C_MASTER_MANAGER_4_H_

