#ifndef _TYPE_I2C_INTERRUPT_H_
#define _TYPE_I2C_INTERRUPT_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeI2cInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeI2cInterrupt
{
  I2C_INTERRUPT_TX_ACTIVE,
  I2C_INTERRUPT_RX_ACTIVE,
  I2C_INTERRUPT_TRANSFER_COMPLETE,
  I2C_INTERRUPT_STOP_DETECTION,
  I2C_INTERRUPT_NACK,
  I2C_INTERRUPT_ERROR,
  I2C_INTERRUPT_NONE
};

#endif  // _TYPE_I2C_INTERRUPT_H_
