#include "Common.h"
#include "MainUsb.h"

#include "ModuleManagerLeds.h"
#include "ModuleManagerUsb.h"
#include "UsbDevicesManager.h"
#include "ModuleManagerLcd.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsb
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainUsb::MainUsb () :
  Main (),
  m_physical_manager (),
  m_clock_control (),
  m_interrupts (),
  m_mouse (),
//  m_keyboard (),
  m_green_led_timer (),
  m_green_led_on (false),
  m_refresh_display (true)
{
  memset (&m_cells, 0, sizeof (m_cells));
  m_cells[8][15].m_radius = 32;
  m_cells[8][15].m_param_x0 = 240;
  m_cells[8][15].m_param_y0 = 136;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsb
// @method:   registerPhaseEnded
// @note:     The green led is turned on once we have finished the 'MODULE_ACTION_START' phase.
// @note:     This is due to the fact that the main manager is always last, as well as tick advancement between phases.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainUsb::registerPhaseStarted (uint32_t action)
{
  if (action == MODULE_ACTION_START)
  {
    ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
    UsbDevicesManager & devices_manager = module_usb.getDevicesManager ();

    devices_manager.registerDevice (m_mouse);
//    devices_manager.registerDevice (m_keyboard);
  }

  ASSERT_TEST (ModuleManager::testModuleActive (MODULE_MANAGER_LEDS));

  if (MODULE_ACTION_TICK < action)
  {
    ledTick ();
    pollMouse ();
    refreshScreen ();
    refreshCells ();
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
      m_green_led_on = true;
      manager_leds.setLed (LED_COLOR_GREEN, true);
      manager_leds.setLed (LED_COLOR_RED, false);
      m_green_led_timer.leaseFromNow (1000);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsb
// @method:   ledTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainUsb::ledTick ()
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
// @class:    MainUsb
// @method:   pollMouse
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainUsb::pollMouse ()
{
  if (!m_mouse.testDeviceActive ())
  {
    return;
  }

  uint16_t x_param = m_mouse.getXLocation ();
  uint16_t y_param = m_mouse.getYLocation ();

  uint16_t cell_x = x_param / 16;
  uint16_t cell_y = y_param / 16;
  ASSERT_CRITICAL (cell_x < 31);
  ASSERT_CRITICAL (cell_y < 18);

  CellData & active_cell = m_cells[cell_y][cell_x];
  if (active_cell.m_radius < 16)
  {
    m_refresh_display = true;
    active_cell.m_radius = 16;
  }

  active_cell.m_param_x0 = x_param;
  active_cell.m_param_y0 = y_param;
  active_cell.m_timeout.leaseFromNow (1000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsb
// @method:   refreshCells
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainUsb::refreshCells ()
{
  for (int y_cell = 0; y_cell <= 17; y_cell++)
  {
    for (int x_cell = 0; x_cell <= 30; x_cell++)
    {
      CellData & active_cell = m_cells[y_cell][x_cell];
      if ((0 < active_cell.m_radius) && (active_cell.m_timeout.testExpired ()))
      {
        m_refresh_display = true;
        active_cell.m_radius = active_cell.m_radius / 2;
        active_cell.m_timeout.leaseFromNow (1000);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainUsb
// @method:   refreshScreen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainUsb::refreshScreen ()
{
  if (m_refresh_display)
  {
    ModuleManagerLcd & lcd_manager = ModuleManager::getLcd ();
    uint16_t * next_base = lcd_manager.getNextScreenGraphics ();
    memset (next_base, 0, 480 * 272 * 2);

    for (int y_cell = 0; y_cell <= 17; y_cell++)
    {
      for (int x_cell = 0; x_cell <= 30; x_cell++)
      {
        CellData & active_cell = m_cells[y_cell][x_cell];
        if (0 < active_cell.m_radius)
        {
          int radius = active_cell.m_radius / 2;
          int center_x = active_cell.m_param_x0;
          int center_y = active_cell.m_param_y0;

          for (int y_offset = -radius; y_offset < radius; y_offset++)
          {
            if (((center_y + y_offset) < 0) || (271 < (center_y + y_offset)))
            {
              continue;
            }

            for (int x_offset = -radius; x_offset < radius; x_offset++)
            {
              if (((center_x + x_offset) < 0) || (479 < (center_x + x_offset)))
              {
                continue;
              }

              int offset = (480 * (center_y + y_offset)) + center_x + x_offset;
              next_base[offset] = 0xFFFFU;
            }
          }
        }
      }
    }

    lcd_manager.displayNextScreen (LCD_ACTIVE_GRAPHICS);

    m_refresh_display = false;
  }
}

