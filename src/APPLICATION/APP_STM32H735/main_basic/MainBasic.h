#ifndef _MAIN_BASIC_H_
#define _MAIN_BASIC_H_

#include "Main.h"
#include "PhysicalModuleManagerPhysical.h"
#include "PlatformModuleManagerClockControl.h"
#include "PhysicalModuleManagerInterrupts.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainBasic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainBasic : public Main
{
public:
  MainBasic ();

private:
  virtual void registerPhaseStarted (uint32_t actions_count) final;
  void ledTick ();

  PhysicalModuleManagerPhysical m_physical_manager;
  PlatformModuleManagerClockControl m_clock_control;
  PhysicalModuleManagerInterrupts m_interrupts;

  Timer m_green_led_timer;
  bool m_green_led_on;
};

typedef MainBasic ApplicationMain;

#endif  // _MAIN_BASIC_H_

