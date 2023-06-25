#ifndef _I2C_MASTER_MANAGER_H_
#define _I2C_MASTER_MANAGER_H_

#include "TypeI2cTransfer.h"
#include "I2cTransferParams.h"
#include "I2cMaster.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @note:     'Interrupt' is a pure virtual interface.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class I2cMasterManager : public I2cMaster
{
public:
  I2cMasterManager ();

  void setupTransaction (uint8_t device_address, 
                         uint8_t bytes_to_send, const uint8_t * buffer_to_send, uint8_t bytes_to_receive);
  // 'tick' returns a pointer to buffer when transaction completed successfully.
  const uint8_t * tick ();

private:
  virtual void handleInterrupt (TypeInterruptAction) final;
  void initTransfer ();
  void markError ();
  void transferStopped ();
  void txInterrupt ();
  void rxInterrupt ();
  void transferCompleteInterrupt ();
  void stopInterrupt ();

  volatile TypeI2cTransfer m_state;
  Timer m_transaction_timeout;
  I2cTransferParams m_transfer_params;
};

#endif  // _I2C_MASTER_MANAGER_H_
