#ifndef _H735_I2C_MASTER_H_
#define _H735_I2C_MASTER_H_

#include "I2cMaster.h"
#include "H735I2cRegisters.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMaster
// @dscr:     Behavior is documented by PM0468 Pragramming manual, section 52.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735I2cMaster : public I2cMaster
{
protected:
  H735I2cMaster (struct H735I2cRegisters &);

  virtual void configure (const uint32_t params[I2C_CONFIGURATION_PARAMS_COUNT]) final;

private:
  virtual int read (I2cMessage &) final;

  void clockEnable ();
  void configureTimingR100K ();
  uint32_t getPclkPicoPeriod () const;
  int setupDataAddress (I2cMessage &);
  int readData (I2cMessage &);
  bool waitIsrBit (uint32_t mask);

  struct H735I2cRegisters & m_i2c_registers;
  Timer m_action_timeout;
};

typedef H735I2cMaster PhysicalI2cMaster;

#endif  // _H735_I2C_MASTER_H_

