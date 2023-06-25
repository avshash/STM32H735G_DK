#include "Common.h"
#include "Disc735I2cMasterManager4.h"

#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735I2cMasterManager4
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Disc735I2cMasterManager4::Disc735I2cMasterManager4 () :
  PhysicalI2cMasterManager (I2C_4_REGISTERS),
  m_scl (GPIO_BANK_F, 14),
  m_sda (GPIO_BANK_F, 15)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735I2cMasterManager4
// @method:   activateGpio
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Disc735I2cMasterManager4::activateGpio ()
{
  m_scl.activate (4, GPIO_SPEED_MEDIUM, GPIO_LOAD_PULL_UP, GPIO_OUTPUT_OPEN_DRAIN);
  m_sda.activate (4, GPIO_SPEED_MEDIUM, GPIO_LOAD_PULL_UP, GPIO_OUTPUT_OPEN_DRAIN);
}
