#ifndef _H735_I2C_MASTER_MANAGER_H_
#define _H735_I2C_MASTER_MANAGER_H_

#include "I2cMasterManager.h"
#include "H735I2cRegisters.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @dscr:     Behavior is documented by PM0468 Pragramming manual, section 52.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735I2cMasterManager : public I2cMasterManager
{
protected:
  H735I2cMasterManager (struct H735I2cRegisters &);

private:
  virtual void configure (uint32_t frequency) final;
  virtual void setupTransfer (uint8_t device_address, uint8_t data_size, bool b_last_action, bool b_action_send) final;
  virtual void disablePeripheral () final;
  virtual TypeI2cInterrupt getActiveInterrupt () final;
  virtual void setNextTxValue (uint8_t to_send) final;
  virtual uint8_t getNextRxValue () final;
  virtual int getInterruptParam (TypeInterruptParam) final;
  virtual int getIrqNumber (int index) final;

  void clockEnable ();
  void configureTimingR100K ();
  uint32_t getPclkPicoPeriod () const;
  void registerInterruptHandler ();
  int getI2cIndex () const;

  struct H735I2cRegisters & m_i2c_registers;
};

typedef H735I2cMasterManager PhysicalI2cMasterManager;

#endif  // _H735_I2C_MASTER_MANAGER_H_

