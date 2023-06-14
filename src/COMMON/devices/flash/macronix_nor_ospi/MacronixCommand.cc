#include "Common.h"
#include "MacronixCommand.h"

#include "MacronixInterface.h"
#include "MacronixInstruction.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixCommand
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MacronixCommand::MacronixCommand (MacronixInterface & interface, TypeMacronixCommand command_type) :
  m_interface (interface)
{
  MacronixInstruction::initParams (m_command, command_type, m_interface.getDummyCycles ());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixCommand
// @method:   configureAddress
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixCommand::configureAddress (uint32_t address)
{
  m_command.m_address_type = OSPI_ADDRESS_BYTES_4;
  m_command.m_address = address;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixCommand
// @method:   writeData
// @return:   Number of bytes sent on success (might be less than 'requested_bytes_count'), negative number on failure.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
MacronixCommand::writeData (const uint8_t * buffer, uint32_t requested_bytes_count)
{
  ASSERT_DEV (((0 < requested_bytes_count) && (buffer != NULL)) ||
              ((0 == requested_bytes_count) && (buffer == NULL)));

  m_command.m_data_bytes_len = MIN ((unsigned int) requested_bytes_count, sizeof (m_command.m_data));
  memcpy (m_command.m_data, buffer, m_command.m_data_bytes_len);

  return m_interface.writeData (m_command);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixCommand
// @method:   readData
// @note:     When not receiving any reply, use 'writeData'.
// @return:   Number of bytes received on success (might be less than 'requested_bytes_count'),
// @return:   negative number on failure.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
MacronixCommand::readData (uint8_t * buffer, uint32_t requested_bytes_count)
{
  ASSERT_CRITICAL (0 < requested_bytes_count);
  ASSERT_DEV (buffer != NULL);

  m_command.m_data_bytes_len = MIN (requested_bytes_count, sizeof (m_command.m_data));
  int bytes_received = m_interface.readData (m_command);

  if (0 < bytes_received)
  {
    memcpy (buffer, m_command.m_data, m_command.m_data_bytes_len);
  }

  return bytes_received;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixCommand
// @method:   testIsSpi
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
MacronixCommand::testIsSpi () const
{
  return (m_command.m_mode == OSPI_MODE_SPI);
}

