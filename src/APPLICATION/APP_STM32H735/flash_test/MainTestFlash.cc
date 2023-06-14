#include "Common.h"
#include "MainTestFlash.h"

#include "ModuleManagerLeds.h"
#include "ModuleManagerFlash.h"
#include "Flash.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainTestFlash::MainTestFlash () :
  Main (),
  m_physical_manager (),
  m_test_phase (FLASH_TEST_PHASE_INIT),
  m_range_start (0),
  m_range_end (0),
  m_address (0),
  m_seed (0)
{
  memset (m_phase_timestamp, 0, sizeof (m_phase_timestamp));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   setNextPhase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::setNextPhase ()
{
  ASSERT_CRITICAL (m_test_phase < FLASH_TEST_PHASE_TEST_GOOD);
  m_phase_timestamp[m_test_phase] = SystemTime::getSystemMilliTime ();

  m_test_phase = (TypeFlashTestPhase) (m_test_phase + 1);

  ASSERT_CRITICAL (ModuleManager::testModuleActive (MODULE_MANAGER_LEDS));
  ModuleManagerLeds & manager_leds = ModuleManager::getLeds ();

  switch (m_test_phase)
  {
    case FLASH_TEST_PHASE_TEST_ERASE_CHIP_0:
    case FLASH_TEST_PHASE_TEST_ERASE_CHIP_1:
    case FLASH_TEST_PHASE_TEST_ERASE_CHIP_2:
      manager_leds.setLed (LED_COLOR_RED, true);
      m_seed = 0x00000000UL;
      break;

    case FLASH_TEST_PHASE_TEST_WRITE_PAGE_1:
      m_seed = 0xA5194081UL;
      break;

    case FLASH_TEST_PHASE_TEST_WRITE_PAGE_2:
      m_seed = 0xD1BC032BUL;
      break;

    case FLASH_TEST_PHASE_TEST_GOOD:
      manager_leds.setLed (LED_COLOR_RED, false);
      manager_leds.setLed (LED_COLOR_GREEN, true);
      break;

    default:
      break;
  }

  m_address = m_range_start;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   registerPhaseStarted
// @note:     The green led is turned on once we have finished the 'MODULE_ACTION_START' phase.
// @note:     This is due to the fact that the main manager is always last, as well as tick advancement between phases.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::registerPhaseStarted (uint32_t actions_count)
{
  if ((m_test_phase < FLASH_TEST_PHASE_TEST_GOOD) && (MODULE_ACTION_TICK <= actions_count))
  {
    // Red led is always active during testing. Green led is blinking 1 second on, 1 second off.
    bool b_green_active = (((SystemTime::getSystemMilliTime () / 1000) & 0x01) != 0);
    ModuleManagerLeds & manager_leds = ModuleManager::getLeds ();
    manager_leds.setLed (LED_COLOR_GREEN, b_green_active);

    testFlash ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   testFlash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::testFlash ()
{
  switch (m_test_phase)
  {
    case FLASH_TEST_PHASE_INIT:
      initTest ();
      break;

    case FLASH_TEST_PHASE_TEST_ERASE_CHIP_0:
      eraseFlash ();
      break;

    case FLASH_TEST_PHASE_TEST_WRITE_PAGE_1:
      testWrite1 ();
      break;

    case FLASH_TEST_PHASE_TEST_VERIFY_PAGE_1:
      verifyMemory ();
      break;

    case FLASH_TEST_PHASE_TEST_ERASE_CHIP_1:
      eraseFlash ();
      break;

    case FLASH_TEST_PHASE_TEST_WRITE_PAGE_2:
      testWrite2 ();
      break;

    case FLASH_TEST_PHASE_TEST_VERIFY_PAGE_2:
      verifyMemory2 ();
      break;

    case FLASH_TEST_PHASE_TEST_ERASE_CHIP_2:
      eraseFlash ();
      break;

    case FLASH_TEST_PHASE_TEST_GOOD:
      ASSERT_CRITICAL (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   initTest
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::initTest ()
{
  uint32_t test_size = 0x10 * getFlashInterface ().getFlashParam (FLASH_PARAM_SECTOR_SIZE);

  m_range_end = getFlashInterface ().getFlashParam (FLASH_PARAM_MEMORY_SIZE);
  if (test_size < m_range_end)
  {
    m_range_start = m_range_end - test_size;
  }
  else
  {
    m_range_start = 0;
  }

  setNextPhase ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   eraseFlash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::eraseFlash ()
{
  Flash & flash_interface = getFlashInterface ();

  flash_interface.resetMemoryRange (m_range_start, m_range_end);
  verifyMemory ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   verifyMemory
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::verifyMemory ()
{
  m_address = m_range_start;

  uint8_t buffer[0x200];
  while (m_address < m_range_end)
  {
    uint32_t address = m_address;
    setupBuffer (buffer, sizeof (buffer));
    testBuffer (address, buffer, sizeof (buffer));
  }

  setNextPhase ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   setupBuffer
// @note:     Advances 'm_address' according to 'bytes_count'.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::setupBuffer (uint8_t * buffer, uint32_t bytes_count)
{
  uint32_t cur_val = m_seed * m_address;

  for (uint32_t offset = 0; offset < bytes_count ; offset++)
  {
    cur_val += m_seed;
    buffer[offset] = (~cur_val) >> 24;
  }

  m_address += bytes_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   testBuffer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::testBuffer (uint32_t address, const uint8_t * expected_data, uint32_t buffer_bytes)
{
  uint8_t actual_data[0x200];

  ASSERT_CRITICAL ((buffer_bytes <= sizeof (actual_data)) && (0 < buffer_bytes));

  Flash & flash_interface = getFlashInterface ();

  flash_interface.readData (address, actual_data, buffer_bytes);

  if (memcmp (actual_data, expected_data, buffer_bytes) != 0)
  {
    ASSERT_CRITICAL (false);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   getFlashInterface
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Flash &
MainTestFlash::getFlashInterface ()
{
  ModuleManagerFlash & flash_manager = ModuleManager::getFlash ();
  return flash_manager.getFlashInterface ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   testWrite1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::testWrite1 ()
{
  uint8_t buffer[0x200];
  ASSERT_CRITICAL ((m_address + sizeof (buffer)) <= m_range_end);

  uint32_t address = m_address;
  setupBuffer (buffer, sizeof (buffer));

  Flash & flash_interface = getFlashInterface ();
  flash_interface.writeData (address, (uint8_t *) buffer, sizeof (buffer));

  if (m_address == m_range_end)
  {
    setNextPhase ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   testWrite2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::testWrite2 ()
{
  uint32_t address = m_address;
  uint8_t buffer[0x200];

  // Setup actual data.
  setupBuffer (buffer, 1);
  setupBuffer (buffer + 1, buffer[0]);

  if (m_range_end < m_address)
  {
    m_address = m_range_end;
  }

  Flash & flash_interface = getFlashInterface ();
  flash_interface.writeData (address, (uint8_t *) buffer, m_address - address);

  // Skip to next section.
  setupBuffer (buffer, 1);
  m_address += buffer[0];

  if (m_range_end <= m_address)
  {
    setNextPhase ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
// @method:   verifyMemory2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestFlash::verifyMemory2 ()
{
  uint8_t reset_buffer[0x100];
  memset (reset_buffer, 0xFF, sizeof (reset_buffer));

  m_address = m_range_start;

  while (m_address < m_range_end)
  {
    uint32_t address = m_address;
    uint8_t buffer[0x100];

    // Setup actual data.
    setupBuffer (buffer, 1);
    setupBuffer (buffer + 1, buffer[0]);

    if (m_range_end < m_address)
    {
      m_address = m_range_end;
    }
    testBuffer (address, buffer, m_address - address);

    // Skip to next section.
    address = m_address;
    setupBuffer (buffer, 1);
    m_address += buffer[0];
    if (m_range_end < m_address)
    {
      m_address = m_range_end;
    }

    if (address < m_address)
    {
      testBuffer (address, reset_buffer, m_address - address);
    }
  }

  setNextPhase ();
}

