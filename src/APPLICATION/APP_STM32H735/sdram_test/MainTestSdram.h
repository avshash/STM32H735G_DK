#ifndef _MAIN_TEST_SDRAM_H_
#define _MAIN_TEST_SDRAM_H_

#include "Main.h"
#include "PhysicalModuleManagerPhysical.h"
#include "PlatformModuleManagerClockControl.h"
#include "PhysicalModuleManagerInterrupts.h"
#include "TypeSdramTestPhase.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestSdram
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainTestSdram : public Main
{
public:
  MainTestSdram ();

private:
  virtual void registerPhaseStarted (uint32_t actions_count) final;

  void setNextPhase ();
  void initSeed ();
  void phaseInit ();
  void testSdram ();
  void setupBuffer (uint8_t * buffer, uint32_t bytes_count);
  void blockSequence (bool b_verify);
  void blockRandom (bool b_verify);

  PhysicalModuleManagerPhysical m_physical_manager;
  PlatformModuleManagerClockControl m_clock_control;
  PhysicalModuleManagerInterrupts m_interrupts;

  TypeSdramTestPhase m_test_phase;
  uint8_t * m_memory_base;
  uint32_t m_memory_size;
  uint32_t m_offset;
  uint32_t m_value_0;
  uint32_t m_seed;
  uint32_t m_phase_timestamp[SDRAM_TEST_PHASE_TEST_GOOD];
};

typedef MainTestSdram ApplicationMain;

#endif  // _MAIN_TEST_SDRAM_H_

