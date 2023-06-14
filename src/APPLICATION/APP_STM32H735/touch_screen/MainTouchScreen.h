#ifndef _MAIN_TOUCH_SCREEN_H_
#define _MAIN_TOUCH_SCREEN_H_

#include "Main.h"
#include "PhysicalModuleManagerPhysical.h"
#include "PlatformModuleManagerClockControl.h"
#include "PhysicalModuleManagerInterrupts.h"
#include "ListenerTouchScreen.h"
#include "Timer.h"
#include "CellData.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTouchScreen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainTouchScreen : public Main, public ListenerTouchScreen
{
public:
  MainTouchScreen ();

private:
  virtual void registerPhaseStarted (uint32_t actions_count) final;
  void ledTick ();
  void registerActive (uint16_t x, uint16_t y);
  void refreshCells ();
  void refreshScreen ();

  virtual void registerAction (const TouchData & last_data, const TouchData & next_data) final;

  PhysicalModuleManagerPhysical m_physical_manager;
  PlatformModuleManagerClockControl m_clock_control;
  PhysicalModuleManagerInterrupts m_interrupts;

  Timer m_green_led_timer;
  bool m_green_led_on;
  bool m_refresh_display;
  CellData m_cells[18][31];
};

typedef MainTouchScreen ApplicationMain;

#endif  // _MAIN_TOUCH_SCREEN_H_

