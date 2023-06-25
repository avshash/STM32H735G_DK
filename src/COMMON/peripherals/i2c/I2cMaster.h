#ifndef _I2C_MASTER_H_
#define _I2C_MASTER_H_

#include "I2c.h"
#include "TypeI2cInterrupt.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMaster
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class I2cMaster : public I2c
{
protected:
  virtual void setupTransfer (uint8_t device_address, uint8_t data_size, bool b_last_action, bool b_action_send) = 0;
  virtual void disablePeripheral () = 0;
  virtual TypeI2cInterrupt getActiveInterrupt () = 0;
  virtual void setNextTxValue (uint8_t to_send) = 0;
  virtual uint8_t getNextRxValue () = 0;
};

#endif  // _I2C_MASTER_H_
