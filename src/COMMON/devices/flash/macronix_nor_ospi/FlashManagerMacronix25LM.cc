#include "Common.h"
#include "FlashManagerMacronix25LM.h"

#include "MacronixActionWrite.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FlashManagerMacronix25LM::FlashManagerMacronix25LM (OctoSpi & flash_interface,
                                                    const MacronixParams & macronix_params) :
  m_macronix_params (macronix_params),
  m_flash_interface (flash_interface, m_macronix_params),
  m_configuration (m_flash_interface)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   doAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FlashManagerMacronix25LM::doAction (TypeModuleAction action)
{
  switch (action)
  {
    case MODULE_ACTION_TICK:
      break;

    case MODULE_ACTION_OPEN:
      ASSERT_DEV (m_macronix_params.verifyInitiated ());
      break;

    case MODULE_ACTION_ACTIVATE_GPIO:
      m_flash_interface.activateGpio ();
      break;

    case MODULE_ACTION_START:
      configureMacronix ();
      break;

    case MODULE_ACTION_CONFIGURE_PLL_CLOCKS:
    case MODULE_ACTION_START_PLL_CLOCKS:
    case MODULE_ACTION_START_MDMA:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   getFlashParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
FlashManagerMacronix25LM::getFlashParam (TypeFlashParam param) const
{
  return m_macronix_params.getFlashParam (param);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   resetMemoryRange
// @note:     Worst time expected is about 6 minutes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FlashManagerMacronix25LM::resetMemoryRange (uint32_t range_start, uint32_t range_limit)
{
  ASSERT_CRITICAL (range_limit <= m_macronix_params.getFlashParam (FLASH_PARAM_MEMORY_SIZE));
  ASSERT_CRITICAL (((range_start & 0x0FFFU) == 0) && ((range_limit & 0x0FFFU) == 0));

  m_configuration.strSetMemoryProtection (false);

  MacronixActionWrite reset_sector_action (m_flash_interface, MACRONIX_COMMAND_OPI_SE);

  // Reset all sectors in range.
  for (uint32_t address = range_start; address < range_limit; address += 0x1000U)
  {
    // Do action only if needed - to save time.
    if (!testSectorReset (address))
    {
      reset_sector_action.writeData (address, NULL, 0);
    }

    ASSERT_CRITICAL (testSectorReset (address));
  }

  m_configuration.strSetMemoryProtection (true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   testSectorReset
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
FlashManagerMacronix25LM::testSectorReset (uint32_t sector_base)
{
  uint8_t buffer[0x100];
  memset (buffer, 0xFF, 0x100);

  bool b_reply = true;

  for (uint32_t offset = 0; offset < 0x1000U; offset += 0x100)
  {
    if (!testPageData (sector_base + offset, buffer, 0x100))
    {
      b_reply = false;
      break;
    }
  }

  return b_reply;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   testPageData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
FlashManagerMacronix25LM::testPageData (uint32_t address, const uint8_t * target_buffer, uint32_t bytes_count)
{
  ASSERT_TEST (bytes_count <= 0x100);

  uint8_t read_buffer[0x100];
  readData (address, read_buffer, bytes_count);

  return (memcmp (read_buffer, target_buffer, bytes_count) == 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   writeData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FlashManagerMacronix25LM::writeData (uint32_t address, const uint8_t * buffer, uint32_t buffer_size)
{
  ASSERT_TEST (0 < buffer_size);
  ASSERT_CRITICAL ((address + buffer_size) <= m_macronix_params.getFlashParam (FLASH_PARAM_MEMORY_SIZE));

  m_configuration.strSetMemoryProtection (false);

  uint32_t bytes_count;
  for (uint32_t offset = 0; offset < buffer_size; offset += bytes_count)
  {
    bytes_count = writePage (address + offset, buffer + offset, buffer_size - offset);
  }

  m_configuration.strSetMemoryProtection (true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   writePage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
FlashManagerMacronix25LM::writePage (uint32_t address, const uint8_t * buffer, uint32_t bytes_count)
{
  uint32_t page_reminder = 0x100 - (address & 0xFF);
  bytes_count = MIN (bytes_count, page_reminder);

  MacronixActionWrite write_command (m_flash_interface, MACRONIX_COMMAND_OPI_PP);

  for (uint32_t offset = 0; offset < bytes_count;)
  {
    offset += write_command.writeData (address + offset, buffer + offset, bytes_count - offset);
  }

  if (!testPageData (address, buffer, bytes_count))
  {
    ASSERT_CRITICAL (false);
  }
    
  return bytes_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   readData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FlashManagerMacronix25LM::readData (uint32_t address, uint8_t * buffer, uint32_t bytes_count)
{
  ASSERT_TEST (0 < bytes_count);
  ASSERT_CRITICAL ((address + bytes_count) <= m_macronix_params.getFlashParam (FLASH_PARAM_MEMORY_SIZE));

  MacronixCommand read_command (m_flash_interface, MACRONIX_COMMAND_OPI_8READ);

  uint32_t address_limit = address + bytes_count;
  while (address < address_limit)
  {
    read_command.configureAddress (address);
    uint32_t bytes_read = read_command.readData (buffer, address_limit - address);

    address += bytes_read;
    buffer += bytes_read;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @method:   configureMacronix
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FlashManagerMacronix25LM::configureMacronix ()
{
  m_flash_interface.enableOspi (m_macronix_params);

  uint32_t chip_id_target = 0;
  ((uint8_t *) &chip_id_target)[0] = m_macronix_params.getMacronixParam (MACRONIX_PARAM_MANUFACTORER_ID) & 0xFF;
  ((uint8_t *) &chip_id_target)[1] = m_macronix_params.getMacronixParam (MACRONIX_PARAM_MEMORY_TYPE) & 0xFF;
  ((uint8_t *) &chip_id_target)[2] = m_macronix_params.getMacronixParam (MACRONIX_PARAM_MEMORY_DENSITY) & 0xFF;
  ASSERT_DEV (chip_id_target != 0);

  m_configuration.setupStrMode (chip_id_target); 
}

