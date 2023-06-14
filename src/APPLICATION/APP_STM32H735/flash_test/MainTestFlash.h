#ifndef _MAIN_TEST_FLASH_H_
#define _MAIN_TEST_FLASH_H_

#include "Main.h"
#include "PhysicalModuleManagerPhysical.h"
#include "PlatformModuleManagerClockControl.h"
#include "PhysicalModuleManagerInterrupts.h"
#include "TypeFlashTestPhase.h"
#include "Timer.h"

class Flash;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTestFlash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainTestFlash : public Main
{
public:
  MainTestFlash ();

private:
  virtual void registerPhaseStarted (uint32_t actions_count) final;

  Flash & getFlashInterface ();
  void setNextPhase ();
  void testFlash ();
  void initTest ();
  void eraseFlash ();
  void verifyMemory ();
  void setupBuffer (uint8_t * buffer, uint32_t bytes_count);
  void testBuffer (uint32_t address, const uint8_t * expected_data, uint32_t buffer_bytes);
  void testWrite1 ();
  void testWrite2 ();
  void verifyMemory2 ();

  PhysicalModuleManagerPhysical m_physical_manager;
  PlatformModuleManagerClockControl m_clock_control;
  PhysicalModuleManagerInterrupts m_interrupts;

  TypeFlashTestPhase m_test_phase;
  uint32_t m_range_start;
  uint32_t m_range_end;
  uint32_t m_address;
  uint32_t m_seed;
  uint32_t m_phase_timestamp[FLASH_TEST_PHASE_TEST_GOOD];
};

typedef MainTestFlash ApplicationMain;

#endif  // _MAIN_TEST_FLASH_H_

