#ifndef _MAIN_USB_PRINTER_H_
#define _MAIN_USB_PRINTER_H_

#include "Main.h"
#include "PhysicalModuleManagerPhysical.h"
#include "PlatformModuleManagerClockControl.h"
#include "PhysicalModuleManagerInterrupts.h"
#include "UsbDeviceSinglePrinterEsc.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsbPrinter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainUsbPrinter : public Main
{
public:
  MainUsbPrinter ();

private:
  virtual void registerPhaseStarted (uint32_t actions_count) final;
  void ledTick ();
  void initFlash ();
  void writeImage (int index);

  PhysicalModuleManagerPhysical m_physical_manager;
  PlatformModuleManagerClockControl m_clock_control;
  PhysicalModuleManagerInterrupts m_interrupts;

  UsbDeviceSinglePrinterEsc m_printer;
  Timer m_green_led_timer;
  bool m_green_led_on;
  bool m_refresh_display;
  uint8_t const * m_image[2];
  uint32_t m_image_address[2];
  uint32_t m_image_size[2];
};

typedef MainUsbPrinter ApplicationMain;

#endif  // _MAIN_USB_PRINTER_H_

