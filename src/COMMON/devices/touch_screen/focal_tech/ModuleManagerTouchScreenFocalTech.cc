#include "Common.h"
#include "ModuleManagerTouchScreenFocalTech.h"

#include "I2cMasterManager.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerTouchScreenFocalTech::ModuleManagerTouchScreenFocalTech (ListenerTouchScreen & listener,
                                                                      I2cMasterManager & interface,
                                                                      bool b_reversed) :
  ModuleManagerTouchScreen (listener),
  m_interface (interface),
  m_reversed (b_reversed),
  m_active (false),
  m_query_time ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
// @method:   doAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerTouchScreenFocalTech::doAction (TypeModuleAction action)
{
  switch (action)
  {
    case MODULE_ACTION_TICK:
      tick ();
      break;

    case MODULE_ACTION_ACTIVATE_GPIO:
      m_interface.activateGpio ();
      break;

    case MODULE_ACTION_START:
      m_interface.configure (100000);
      verifyId ();
      break;

    case MODULE_ACTION_OPEN:
    case MODULE_ACTION_CONFIGURE_PLL_CLOCKS:
    case MODULE_ACTION_START_PLL_CLOCKS:
    case MODULE_ACTION_START_MDMA:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
// @method:   verifyId
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerTouchScreenFocalTech::verifyId ()
{
  uint8_t register_address = 0xA8;

  m_interface.setupTransaction (0x38, 1, &register_address, 1);
  uint8_t const * reply = NULL;
  while (reply == NULL)
  {
    reply = m_interface.tick ();
  }

  ASSERT_CRITICAL (*reply == 0x51);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
// @method:   tick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerTouchScreenFocalTech::tick ()
{
  if (m_active)
  {
    const uint8_t * reply = m_interface.tick ();

    if (reply != NULL)
    {
      handleReply (reply);

      // Set next query to 100 ms.
      m_active = false;
      m_query_time.leaseFromNow (100);
    }
  }
  else
  {
    if (m_query_time.testExpired ())
    {
      uint8_t register_address = 0x02;

      // Query touch screen for 11 bytes.
      m_interface.setupTransaction (0x38, 1, &register_address, 11);

      m_active = true;
    }
  }
}
  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
// @method:   handleReply
// @note:     Query touch screen for 11 bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerTouchScreenFocalTech::handleReply (const uint8_t * reply)
{
  TouchData current_data;
  memset (&current_data, 0, sizeof (current_data));

  current_data.m_touch_count = reply[0];

  if (current_data.m_touch_count == 0xFFU)
  {
    // On wakeup, FocalTech chips initialize registers to 0xFF.
    current_data.m_touch_count = 0;
  }

  if (0 < current_data.m_touch_count)
  {
    uint16_t param_x = ((((uint16_t) reply[1]) & 0x0F) << 8) + reply[2];
    uint16_t param_y = ((((uint16_t) reply[3]) & 0x0F) << 8) + reply[4];

    current_data.m_param_x0 = m_reversed ? param_y : param_x;
    current_data.m_param_y0 = m_reversed ? param_x : param_y;
  }

  if (1 < current_data.m_touch_count)
  {
    uint16_t param_x = ((((uint16_t) reply[7]) & 0x0F) << 8) + reply[8];
    uint16_t param_y = ((((uint16_t) reply[9]) & 0x0F) << 8) + reply[10];

    current_data.m_param_x1 = m_reversed ? param_y : param_x;
    current_data.m_param_y1 = m_reversed ? param_x : param_y;
  }

  registerData (current_data);
}

