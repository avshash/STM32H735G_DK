#include "Common.h"
#include "MacronixActionWrite.h"

#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixActionWrite
// @method:   constructor
// @note:     When 'dummy_cycles' is '0', commands are set to SPI. Otherwise, commands are set to OPI.
// @note:     For write actions, dummy_cycles is always set to '0'. Relevent only to the RDSR query.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MacronixActionWrite::MacronixActionWrite (MacronixInterface & interface, TypeMacronixCommand command) :
  m_action (interface, command),
  m_wren (interface, (m_action.testIsSpi () ? MACRONIX_COMMAND_SPI_WREN : MACRONIX_COMMAND_OPI_WREN)),
  m_rdsr (interface, (m_action.testIsSpi () ? MACRONIX_COMMAND_SPI_RDSR : MACRONIX_COMMAND_OPI_RDSR))
{
  if (MACRONIX_COMMAND_SPI_NOP < command)
  {
    m_rdsr.configureAddress (0x00000000UL);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixActionWrite
// @method:   testWriteSuccess
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
MacronixActionWrite::testWriteSuccess ()
{
  // Sets a time limit of 1 second for this action. (should never fail as long as operation mode is correct).
  Timer sentinel;
  sentinel.leaseFromNow (1000);

  uint8_t status_register = 0x03;
  m_rdsr.readData (&status_register, 1);

  while ((status_register & 0x01) != 0x00)
  {
    ASSERT_CRITICAL (!sentinel.testExpired ());

    // Note that when read command fails, 'status_register' is not updated.
    m_rdsr.readData (&status_register, 1);
  }

  return ((status_register & 0x03) == 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixActionWrite
// @method:   writeData
// @return:   Number of bytes actually written.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int
MacronixActionWrite::writeData (const uint8_t * buffer, unsigned int requested_bytes_count)
{
  unsigned int actual_bytes_count = 0;

  for (int tries = 0; tries < 3; tries++)
  {
    // Set write enable.
    m_wren.writeData (NULL, 0);

    actual_bytes_count = m_action.writeData (buffer, requested_bytes_count);

    if (testWriteSuccess ())
    {
      break;
    }

    // Allow only 3 tries.
    ASSERT_CRITICAL (tries < 2);
  }

  return actual_bytes_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixActionWrite
// @method:   writeData
// @return:   Number of bytes actually written.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int
MacronixActionWrite::writeData (uint32_t address, const uint8_t * buffer, unsigned int requested_bytes_count)
{
  m_action.configureAddress (address);

  return writeData (buffer, requested_bytes_count);
}

