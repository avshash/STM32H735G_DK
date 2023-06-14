#include "Common.h"
#include "MainBasic.h"

#include "TextSingleLine.h"
#include "ModuleManagerLeds.h"
#include "ModuleManagerLcd.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainBasic
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainBasic::MainBasic () :
  m_physical_manager (),
  m_green_led_timer (),
  m_green_led_on (false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MainBasic
// @method:   registerPhaseEnded
// @note:     The green led is turned on once we have finished the 'MODULE_ACTION_START' phase.
// @note:     This is due to the fact that the main manager is always last, as well as tick advancement between phases.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainBasic::registerPhaseStarted (uint32_t action)
{
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
// @class:    MainBasic
// @method:   ledTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MainBasic::ledTick ()
{
  if (m_green_led_timer.testExpired ())
  {
    m_green_led_timer.leaseFromNow (1000);
    m_green_led_on = !m_green_led_on;

    ModuleManagerLeds & manager_leds = ModuleManager::getLeds ();
    manager_leds.setLed (LED_COLOR_GREEN, m_green_led_on);

    TextSingleLine ().print ("Next led tick") << (m_green_led_on ? TEXT_COLOR_GREEN_BLACK : TEXT_COLOR_BLUE_BLACK)
                                              << (m_green_led_on ? " on." : " off.");
  }
}

