#include "Common.h"

#include "ApplicationModuleManagerMain.h"

#ifdef _MODULE_LED_MANAGER
#include "PlatformModuleManagerLeds.h"
#endif  // _MODULE_LED_MANAGER

#ifdef _MODULE_FLASH_MANAGER
#include "PlatformModuleManagerFlash.h"
#endif  // _MODULE_FLASH_MANAGER

#ifdef _MODULE_SDRAM_MANAGER
#include "PlatformModuleManagerSdram.h"
#endif  // _MODULE_SDRAM_MANAGER

#ifdef _MODULE_LCD_MANAGER
#include "PlatformModuleManagerLcd.h"
#endif  // _MODULE_LCD_MANAGER

#ifdef _MODULE_TOUCH_SCREEN_MANAGER
#include "PlatformModuleManagerTouchScreen.h"
#endif  // _MODULE_TOUCH_SCREEN_MANAGER

#ifdef _MODULE_USB_MODULE
#include "PlatformModuleManagerUsb.h"
#endif  // _MODULE_USB_MODULE

int
main ()
{
  // 'main manager' initialization must be the first main action.
  ApplicationMain main_application;

  uint32_t static_size = sizeof (main_application);

#ifdef _MODULE_LED_MANAGER
  PlatformModuleManagerLeds leds_manager;
  static_size += sizeof (leds_manager);
#endif  // _MODULE_LED_MANAGER

#ifdef _MODULE_FLASH_MANAGER
  PlatformModuleManagerFlash flash_manager;
  static_size += sizeof (flash_manager);
#endif  // _MODULE_FLASH_MANAGER

#ifdef _MODULE_SDRAM_MANAGER
  PlatformModuleManagerSdram sdram_manager;
  static_size += sizeof (sdram_manager);
#endif  // _MODULE_SDRAM_MANAGER

#ifdef _MODULE_LCD_MANAGER
  PlatformModuleManagerLcd lcd_manager;
  static_size += sizeof (lcd_manager);
#endif  // _MODULE_LCD_MANAGER

#ifdef _MODULE_TOUCH_SCREEN_MANAGER
  PlatformModuleManagerTouchScreen touch_screen_manager (main_application);
  static_size += sizeof (touch_screen_manager);
#endif  // _MODULE_TOUCH_SCREEN_MANAGER

#ifdef _MODULE_USB_MODULE
  PlatformModuleManagerUsb usb_manager;
  static_size += sizeof (usb_manager);
#endif  // _MODULE_USB_MODULE

  main_application.main (static_size);

  return 0;
}

