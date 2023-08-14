#include "Common.h"
#include "MainUsbPrinter.h"

#include "UsbPrinterImage.h"

#include "ModuleManagerLeds.h"
#include "ModuleManagerUsb.h"
#include "UsbDevicesManager.h"
#include "ModuleManagerLcd.h"
#include "TextSingleLine.h"
#include "ModuleManagerFlash.h"
#include "Flash.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsbPrinter
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainUsbPrinter::MainUsbPrinter () :
  Main (),
  m_physical_manager (),
  m_clock_control (),
  m_interrupts (),
  m_printer (),
  m_green_led_timer (),
  m_green_led_on (false),
  m_refresh_display (true)
{
  memset (m_image, 0, sizeof (m_image));
  memset (m_image_address, 0, sizeof (m_image_address));
  memset (m_image_size, 0, sizeof (m_image_size));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsbPrinter
// @method:   registerPhaseEnded
// @note:     The green led is turned on once we have finished the 'MODULE_ACTION_START' phase.
// @note:     This is due to the fact that the main manager is always last, as well as tick advancement between phases.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainUsbPrinter::registerPhaseStarted (uint32_t action)
{
  static int counter = 0;

  if (action == MODULE_ACTION_START)
  {
    ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
    UsbDevicesManager & devices_manager = module_usb.getDevicesManager ();

    devices_manager.registerDevice (m_printer);
  }

  ASSERT_TEST (ModuleManager::testModuleActive (MODULE_MANAGER_LEDS));

  {
    if ((counter < 10) && (m_printer.getPrinterState () == USB_PRINTER_IDLE))
    {
      if (counter == 5)
      {
        m_printer.printQrCode ("0123456789", 10);
      }
      else if (counter == 7)
      {
        uint32_t lines_count = m_image_size[0] / 72;
        m_printer.printRawData (72, lines_count, m_image_address[0]);
      }
      else if (counter == 8)
      {
        uint32_t lines_count = m_image_size[1] / 72;
        m_printer.printRawData (72, lines_count, m_image_address[1]);
      }
      else if (counter == 9)
      {
        m_printer.printCutPaper ();
      }
      else
      {
        char message[10] = {'T', 'e', 's', 't', ' ', 0, 0};
        message[5] = '0' + counter;
        m_printer.printSingleLine (message);
      }
      counter++;
    }
  }

  if (MODULE_ACTION_TICK < action)
  {
    ledTick ();
  }
  else
  {
    ModuleManagerLeds & manager_leds = ModuleManager::getLeds ();

    if (action == (MODULE_ACTION_ACTIVATE_GPIO + 1))
    {
      m_green_led_on = false;
      manager_leds.setLed (LED_COLOR_GREEN, false);
      manager_leds.setLed (LED_COLOR_RED, true);
    }

    if (action == MODULE_ACTION_TICK)
    {
      initFlash ();
      m_green_led_on = true;
      manager_leds.setLed (LED_COLOR_GREEN, true);
      manager_leds.setLed (LED_COLOR_RED, false);
      m_green_led_timer.leaseFromNow (1000);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsbPrinter
// @method:   ledTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainUsbPrinter::ledTick ()
{
  if (m_green_led_timer.testExpired ())
  {
    m_green_led_timer.leaseFromNow (1000);
    m_green_led_on = !m_green_led_on;

    ModuleManagerLeds & manager_leds = ModuleManager::getLeds ();
    manager_leds.setLed (LED_COLOR_GREEN, m_green_led_on);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsbPrinter
// @method:   initFlash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainUsbPrinter::initFlash ()
{
  TextSingleLine ().print ("Init flash verification.");

  ModuleManagerFlash & flash_manager = ModuleManager::getFlash ();
  Flash & flash_interface = flash_manager.getFlashInterface ();
  uint32_t flash_size = flash_interface.getFlashParam (FLASH_PARAM_MEMORY_SIZE);

  m_image[0] = g_usb_printer_image_a;
  m_image_address[0] = (flash_size / 16) * 9;
  m_image_size[0] = sizeof (g_usb_printer_image_a);

  m_image[1] = g_usb_printer_image_b;
  m_image_address[1] = (flash_size / 16) * 10;
  m_image_size[1] = sizeof (g_usb_printer_image_b);

  ASSERT_CRITICAL ((m_image_size[0] + m_image_address[0]) < m_image_address[1]);
  ASSERT_CRITICAL (m_image_size[1] <= flash_size);

  writeImage (0);
  writeImage (1);
  TextSingleLine ().print ("Flash verification completed.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsbPrinter
// @method:   writeImage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainUsbPrinter::writeImage (int index)
{
  ModuleManagerFlash & flash_manager = ModuleManager::getFlash ();
  Flash & flash_interface = flash_manager.getFlashInterface ();

  uint32_t sector_size = flash_interface.getFlashParam (FLASH_PARAM_SECTOR_SIZE);
  const uint8_t * image_base = m_image[index];
  uint32_t image_size = m_image_size[index];
  uint32_t address_base = m_image_address[index];

  bool b_different = false;
  uint8_t buffer[0x100];
  for (uint32_t offset = 0; offset < image_size; offset += 0x100)
  {
    flash_interface.readData (address_base + offset, buffer, 0x100);
    uint32_t test_size = image_size - offset;
    test_size = MIN (0x100, test_size);
    if (memcmp (buffer, image_base + offset, test_size) != 0)
    {
      b_different = true;
      break;
    }
  }

  if (!b_different)
  {
    return;
  }

  uint32_t image_size_rounded = sector_size * ((image_size + sector_size - 1) / sector_size);

  flash_interface.resetMemoryRange (address_base, address_base + image_size_rounded);
  flash_interface.writeData (address_base, image_base, image_size);

  TextSingleLine ().print ("Updated image ") << index << " in flash memory.";
}

