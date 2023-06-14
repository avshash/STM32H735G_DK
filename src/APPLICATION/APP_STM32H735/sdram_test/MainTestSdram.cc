#include "Common.h"
#include "MainTestSdram.h"

#include "ModuleManagerLeds.h"
#include "ModuleManagerSdram.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainTestSdram::MainTestSdram () :
  Main (),
  m_physical_manager (),
  m_test_phase (SDRAM_TEST_PHASE_INIT),
  m_memory_base (NULL),
  m_memory_size (0),
  m_offset (0),
  m_seed (0)
{
  memset (m_phase_timestamp, 0, sizeof (m_phase_timestamp));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
// @method:   setNextPhase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestSdram::setNextPhase ()
{
  ASSERT_CRITICAL (m_test_phase < SDRAM_TEST_PHASE_TEST_GOOD);
  m_phase_timestamp[m_test_phase] = SystemTime::getSystemMilliTime ();

  m_test_phase = (TypeSdramTestPhase) (m_test_phase + 1);

  ASSERT_CRITICAL (ModuleManager::testModuleActive (MODULE_MANAGER_LEDS));
  ModuleManagerLeds & manager_leds = ModuleManager::getLeds ();
  manager_leds.setLed (LED_COLOR_RED, (m_test_phase != SDRAM_TEST_PHASE_TEST_GOOD));

  m_offset = 0;
  initSeed ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
// @method:   initSeed
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestSdram::initSeed ()
{
  switch (m_test_phase)
  {
    case SDRAM_TEST_PHASE_INIT:
      ASSERT_CRITICAL (false);
      break;

    case SDRAM_TEST_PHASE_TEST_SET_00:
    case SDRAM_TEST_PHASE_VERIFY_SET_00:
      m_value_0 = 0;
      m_seed = 0;
      break;

    case SDRAM_TEST_PHASE_TEST_SET_FF:
    case SDRAM_TEST_PHASE_VERIFY_SET_FF:
      m_value_0 = 0xFFFFFFFFUL;
      m_seed = 0;
      break;

    case SDRAM_TEST_PHASE_TEST_SEQUENCE:
    case SDRAM_TEST_PHASE_VERIFY_SEQUENCE:
      m_value_0 = 0;
      m_seed = 0xA5194081UL;     // Seed is a prime number.
      break;

    case SDRAM_TEST_PHASE_TEST_RANDOM:
    case SDRAM_TEST_PHASE_VERIFY_RANDOM:
      m_value_0 = 0xFFFFFFFFUL;
      m_seed = 0xD1BC032BUL;     // Seed is a prime number.
      break;

    case SDRAM_TEST_PHASE_TEST_GOOD:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
// @method:   registerPhaseStarted
// @note:     The green led is turned on once we have finished the 'MODULE_ACTION_START' phase.
// @note:     This is due to the fact that the main manager is always last, as well as tick advancement between phases.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestSdram::registerPhaseStarted (uint32_t actions_count)
{
  // Red led is always active during testing. Green led is blinking 1 second on, 1 second off.
  bool b_green_active = (((SystemTime::getSystemMilliTime () / 1000) & 0x01) != 0);
  ModuleManagerLeds & manager_leds = ModuleManager::getLeds ();
  manager_leds.setLed (LED_COLOR_GREEN, b_green_active);

  if ((m_test_phase < SDRAM_TEST_PHASE_TEST_GOOD) && (MODULE_ACTION_TICK <= actions_count))
  {
    testSdram ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
// @method:   testSdram
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestSdram::testSdram ()
{
  switch (m_test_phase)
  {
    case SDRAM_TEST_PHASE_INIT:
      phaseInit ();
      break;

    case SDRAM_TEST_PHASE_TEST_SET_00:
      blockSequence (false);
      break;

    case SDRAM_TEST_PHASE_VERIFY_SET_00:
      blockSequence (true);
      break;

    case SDRAM_TEST_PHASE_TEST_SET_FF:
      blockSequence (false);
      break;

    case SDRAM_TEST_PHASE_VERIFY_SET_FF:
      blockSequence (true);
      break;

    case SDRAM_TEST_PHASE_TEST_SEQUENCE:
      blockSequence (false);
      break;

    case SDRAM_TEST_PHASE_VERIFY_SEQUENCE:
      blockSequence (true);
      break;

    case SDRAM_TEST_PHASE_TEST_RANDOM:
      blockRandom (false);
      break;

    case SDRAM_TEST_PHASE_VERIFY_RANDOM:
      blockRandom (true);
      break;

    case SDRAM_TEST_PHASE_TEST_GOOD:
      ASSERT_CRITICAL (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
// @method:   phaseInit
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestSdram::phaseInit ()
{
  ModuleManagerSdram & sdram_manager = ModuleManager::getSdram ();
  m_memory_base = (uint8_t *) (sdram_manager.getMemoryBase ());
  m_memory_size = sdram_manager.getMemorySize ();

  setNextPhase ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
// @method:   setupBuffer
// @note:     Advances 'm_offset' according to 'bytes_count'.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestSdram::setupBuffer (uint8_t * buffer, uint32_t bytes_count)
{
  uint32_t cur_val = m_value_0 + (m_seed * m_offset);

  for (uint32_t offset = 0; offset < bytes_count ; offset++)
  {
    cur_val += m_seed;
    buffer[offset] = (~cur_val) >> 24;
  }

  m_offset += bytes_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
// @method:   blockSequence
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestSdram::blockSequence (bool b_verify)
{
  uint8_t * address = m_memory_base + m_offset;

  uint8_t buffer[0x200];
  setupBuffer (buffer, sizeof (buffer));

  if (b_verify)
  {
    if (memcmp (buffer, address, sizeof (buffer)) != 0)
    {
      ASSERT_CRITICAL (false);
    }
  }
  else
  {
    memcpy (address, buffer, sizeof (buffer));
  }

  if (m_memory_size <= m_offset)
  {
    setNextPhase ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
// @method:   blockRandom
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTestSdram::blockRandom (bool b_verify)
{
  uint8_t * address = m_memory_base + ((m_offset * 0xA5194081UL) % m_memory_size);

  uint8_t next_val;
  setupBuffer (&next_val, 1);

  if (b_verify)
  {
    if (next_val != *address)
    {
      while (m_phase_timestamp != (uint32_t *) 0xFFFFFFFFUL);
      ASSERT_CRITICAL (false);
    }
  }
  else
  {
    *address = next_val;
  }

  if (m_memory_size <= m_offset)
  {
    setNextPhase ();
  }
}

