#include "Common.h"
#include "I2cMasterManager.h"

#include "I2cMaster.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
I2cMasterManager::I2cMasterManager () :
  m_state (I2C_TRANSFER_IDLE),
  m_transaction_timeout ()
{
  memset (&m_transfer_params, 0, sizeof (m_transfer_params));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   setupTransaction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
I2cMasterManager::setupTransaction (uint8_t device_address,
                                    uint8_t bytes_to_send, const uint8_t * buffer_to_send, uint8_t bytes_to_receive)
{
  if (m_state != I2C_TRANSFER_IDLE)
  {
    ASSERT_TEST (false);
    return;
  }

  ASSERT_CRITICAL ((bytes_to_send <= sizeof (m_transfer_params.m_buffer)) && 
                   (bytes_to_receive <= sizeof (m_transfer_params.m_buffer)));
  ASSERT_TEST ((0 < bytes_to_send) ||  (0 < bytes_to_receive));

  m_transfer_params.m_device_address = device_address;
  m_transfer_params.m_bytes_to_send = bytes_to_send;
  m_transfer_params.m_bytes_to_receive = bytes_to_receive;
  memcpy (m_transfer_params.m_buffer, buffer_to_send, bytes_to_send);

  initTransfer ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   tick
// @return:   'NULL' when idle, or active. Otherwise, pointer to buffer (once only).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const uint8_t *
I2cMasterManager::tick ()
{
  uint8_t * ret_val = (m_state == I2C_TRANSFER_GOOD) ? m_transfer_params.m_buffer : NULL;

  switch (m_state)
  {
    case I2C_TRANSFER_IDLE:
      break;

    case I2C_TRANSFER_ACTIVE:
      if (m_transaction_timeout.testExpired ())
      {
        markError ();
      }
      break;

    case I2C_TRANSFER_STOPPED:
      transferStopped ();
      break;

    case I2C_TRANSFER_GOOD:
      // Note that at this point the interface is stopped, so there can be no interrupts.
      m_state = I2C_TRANSFER_IDLE;
      m_transfer_params.m_errors_count = 0;
      break;
  }

  return ret_val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   handleInterrupt
// @dscr:     This is the interrupt handler.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
I2cMasterManager::handleInterrupt (TypeInterruptAction)
{
  TypeI2cInterrupt active_interrupt = getActiveInterrupt ();

  switch (active_interrupt)
  {
    case I2C_INTERRUPT_TX_ACTIVE:
      return txInterrupt ();

    case I2C_INTERRUPT_RX_ACTIVE:
      return rxInterrupt ();

    case I2C_INTERRUPT_TRANSFER_COMPLETE:
      return transferCompleteInterrupt ();

    case I2C_INTERRUPT_STOP_DETECTION:
      return stopInterrupt ();

    case I2C_INTERRUPT_NACK:
    case I2C_INTERRUPT_ERROR:
      return markError ();

    case I2C_INTERRUPT_NONE:
      return;
  }

  ASSERT_TEST (false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   initTransfer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
I2cMasterManager::initTransfer ()
{
  // Set timeout for 100Khz - 100 bits pers millisecond.
  uint32_t timeout = 3 + ((m_transfer_params.m_bytes_to_send + m_transfer_params.m_bytes_to_receive) / 10);
  m_transaction_timeout.leaseFromNow (timeout);

  ASSERT_TEST ((m_state == I2C_TRANSFER_IDLE) || (0 < m_transfer_params.m_errors_count));

  m_transfer_params.m_buffer_offset = 0;

  m_state = I2C_TRANSFER_ACTIVE;

  uint8_t device_address = m_transfer_params.m_device_address;
  if (m_transfer_params.m_bytes_to_send == 0)
  {
    uint8_t data_size = m_transfer_params.m_bytes_to_receive;
    setupTransfer (device_address, data_size, true, false);
  }
  else
  {
    uint8_t data_size = m_transfer_params.m_bytes_to_send;
    bool b_last_action = (m_transfer_params.m_bytes_to_receive == 0);
    setupTransfer (device_address, data_size, b_last_action, true);
  }
  // From this point on, the interrupts are enabled.
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   markError
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
I2cMasterManager::markError ()
{
  disablePeripheral ();
  // From this point on, the interrupts are disabled.

  m_state = I2C_TRANSFER_STOPPED;

  m_transfer_params.m_errors_count++;
  ASSERT_CRITICAL (m_transfer_params.m_errors_count < 3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   transferStopped
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
I2cMasterManager::transferStopped ()
{
  if (0 < m_transfer_params.m_errors_count)
  {
    initTransfer ();
  }
  else
  {
    ASSERT_TEST (m_transfer_params.m_buffer_offset == m_transfer_params.m_bytes_to_receive);

    m_state = I2C_TRANSFER_GOOD;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   txInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
I2cMasterManager::txInterrupt ()
{
  uint8_t next_value = m_transfer_params.m_buffer[m_transfer_params.m_buffer_offset];
  setNextTxValue (next_value);
  m_transfer_params.m_buffer_offset++;

  // If sent last byte, we would invoke no more tx interrupts.
  if (m_transfer_params.m_buffer_offset == m_transfer_params.m_bytes_to_send)
  {
    m_transfer_params.m_buffer_offset = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   rxInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
I2cMasterManager::rxInterrupt ()
{
  m_transfer_params.m_buffer[m_transfer_params.m_buffer_offset] = getNextRxValue ();
  m_transfer_params.m_buffer_offset++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   transferCompleteInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
I2cMasterManager::transferCompleteInterrupt ()
{
  if ((m_transfer_params.m_buffer_offset == 0) && (0 < m_transfer_params.m_bytes_to_receive))
  {
    setupTransfer (m_transfer_params.m_device_address, m_transfer_params.m_bytes_to_receive, true, false);
  }
  else
  {
    // Transfer complete interrupt should be disabled when previous action was marked as last.
    ASSERT_TEST (false);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    I2cMasterManager
// @method:   stopInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
I2cMasterManager::stopInterrupt ()
{
  ASSERT_TEST (m_state == I2C_TRANSFER_ACTIVE);

  disablePeripheral ();
  // From this point on, the interrupts are disabled.

  m_state = I2C_TRANSFER_STOPPED;
}

