#include "Common.h"
#include "MainTouchScreen.h"

#include "TextSingleLine.h"
#include "ModuleManagerLeds.h"
#include "ModuleManagerLcd.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTouchScreen
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainTouchScreen::MainTouchScreen () :
  m_physical_manager (),
  m_green_led_timer (),
  m_green_led_on (false),
  m_refresh_display (true)
{
  memset (&m_cells, 0, sizeof (m_cells));
  m_cells[8][14].m_radius = 32;
  m_cells[8][14].m_param_x0 = 232;
  m_cells[8][14].m_param_y0 = 136;
  m_cells[9][15].m_radius = 32;
  m_cells[9][15].m_param_x0 = 248;
  m_cells[9][15].m_param_y0 = 152;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTouchScreen
// @method:   registerPhaseEnded
// @note:     The green led is turned on once we have finished the 'MODULE_ACTION_START' phase.
// @note:     This is due to the fact that the main manager is always last, as well as tick advancement between phases.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTouchScreen::registerPhaseStarted (uint32_t action)
{
  if (MODULE_ACTION_TICK < action)
  {
    refreshScreen ();
    refreshCells ();
  }

  if (ModuleManager::testModuleActive (MODULE_MANAGER_LEDS))
  {
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
        m_green_led_on = true;
        manager_leds.setLed (LED_COLOR_GREEN, true);
        manager_leds.setLed (LED_COLOR_RED, false);
        m_green_led_timer.leaseFromNow (1000);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTouchScreen
// @method:   ledTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTouchScreen::ledTick ()
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
// @class:    MainTouchScreen
// @method:   registerAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTouchScreen::registerAction (const TouchData & last_data, const TouchData & next_data)
{
  if (2 < next_data.m_touch_count)
  {
    TextSingleLine ().print (TEXT_COLOR_RED_BLACK) << "Aborting demo because of too many touches (" <<
                                                      next_data.m_touch_count << ").";
    ASSERT_TEST (false);
  }

  TextSingleLine message (TEXT_COLOR_PURPLE_BLACK);
  message << TEXT_STYLE_DEC1 << "Change touch from {" << last_data.m_touch_count << " touches";
  message << TEXT_STYLE_DEC3 << " (" << last_data.m_param_x0 << ", " << last_data.m_param_y0 << ")";
  message << TEXT_STYLE_DEC3 << " (" << last_data.m_param_x1 << ", " << last_data.m_param_y1 << ")}";
  message << TEXT_STYLE_DEC1 << " to {" << next_data.m_touch_count << " touches";
  message << TEXT_STYLE_DEC3 << " (" << next_data.m_param_x0 << ", " << next_data.m_param_y0 << ")";
  message << TEXT_STYLE_DEC3 << " (" << next_data.m_param_x1 << ", " << next_data.m_param_y1 << ")}";

  if (0 < next_data.m_touch_count)
  {
    registerActive (next_data.m_param_x0, next_data.m_param_y0);
  }

  if (1 < next_data.m_touch_count)
  {
    registerActive (next_data.m_param_x1, next_data.m_param_y1);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTouchScreen
// @method:   registerActive
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTouchScreen::registerActive (uint16_t x, uint16_t y)
{
  uint16_t cell_x = x / 16;
  uint16_t cell_y = y / 16;
  while ((479 < x) || (271 < y));
  ASSERT_CRITICAL (cell_x < 31);
  ASSERT_CRITICAL (cell_y < 18);

  CellData & active_cell = m_cells[cell_y][cell_x];
  if (active_cell.m_radius < 16)
  {
    m_refresh_display = true;
    active_cell.m_radius = 16;
  }

  active_cell.m_param_x0 = x;
  active_cell.m_param_y0 = y;
  active_cell.m_timeout.leaseFromNow (1000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainTouchScreen
// @method:   refreshCells
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTouchScreen::refreshCells ()
{
  for (int y_cell = 0; y_cell < 17; y_cell++)
  {
    for (int x_cell = 0; x_cell < 30; x_cell++)
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
// @class:    MainTouchScreen
// @method:   refreshScreen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainTouchScreen::refreshScreen ()
{
  if (m_refresh_display)
  {
    ModuleManagerLcd & lcd_manager = ModuleManager::getLcd ();
    uint16_t * next_base = lcd_manager.getNextScreenGraphics ();
    memset (next_base, 0, 480 * 272 * 2);

    for (int y_cell = 0; y_cell < 17; y_cell++)
    {
      for (int x_cell = 0; x_cell < 30; x_cell++)
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

