#ifndef _MAIN_USB_HID_H_
#define _MAIN_USB_HID_H_

#include "Main.h"
#include "PhysicalModuleManagerPhysical.h"
#include "PlatformModuleManagerClockControl.h"
#include "PhysicalModuleManagerInterrupts.h"
#include "UsbDeviceSingleHidKeyboard.h"
#include "UsbDeviceSingleHidMouse.h"
#include "Timer.h"
#include "CellData.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsbHid
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainUsbHid : public Main
{
public:
  MainUsbHid ();

private:
  virtual void registerPhaseStarted (uint32_t actions_count) final;
  void ledTick ();
  void pollMouse ();
  void refreshCells ();
  void refreshScreen ();

  PhysicalModuleManagerPhysical m_physical_manager;
  PlatformModuleManagerClockControl m_clock_control;
  PhysicalModuleManagerInterrupts m_interrupts;

  UsbDeviceSingleHidMouse m_mouse;
  UsbDeviceSingleHidKeyboard m_keyboard;
  Timer m_green_led_timer;
  bool m_green_led_on;
  bool m_refresh_display;
  CellData m_cells[18][31];

};

typedef MainUsbHid ApplicationMain;

#endif  // _MAIN_USB_HID_H_

