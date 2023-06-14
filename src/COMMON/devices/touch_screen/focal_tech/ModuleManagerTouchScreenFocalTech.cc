#include "Common.h"
#include "ModuleManagerTouchScreenFocalTech.h"

#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerTouchScreenFocalTech::ModuleManagerTouchScreenFocalTech (ListenerTouchScreen & listener,
                                                                      I2cMaster & interface,
                                                                      bool b_reversed) :
  ModuleManagerTouchScreen (listener),
  m_interface (interface),
  m_reversed (b_reversed),
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
      activateGpio ();
      break;

    case MODULE_ACTION_START:
      startModule ();
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
// @method:   activateGpio
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerTouchScreenFocalTech::activateGpio ()
{
  m_interface.activateGpio ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
// @method:   startModule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerTouchScreenFocalTech::startModule ()
{
  uint32_t params[I2C_CONFIGURATION_PARAMS_COUNT];

  memset (params, 0, sizeof (params));
  params[I2C_CONFIGURATION_PARAM_FREQUENCY] = 100000;

  m_interface.configure (params);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
// @method:   verifyId
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerTouchScreenFocalTech::verifyId ()
{
  I2cMessage query = {0x38, 0xA8, 1};

  m_interface.read (query);
  ASSERT_CRITICAL (query.m_message_data[0] == 0x51);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
// @method:   tick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerTouchScreenFocalTech::tick ()
{
  if (m_query_time.testExpired ())
  {
    I2cMessage query = {0x38, 0x02, 11};
  
    m_interface.read (query);

    TouchData current_data;
    memset (&current_data, 0, sizeof (current_data));
    current_data.m_touch_count = query.m_message_data[0];

    if (current_data.m_touch_count == 0xFFU)
    {
      // On wakeup, FocalTech chips initialize registers to 0xFF.
      current_data.m_touch_count = 0;
    }

    if (0 < current_data.m_touch_count)
    {
      uint16_t param_x = ((((uint16_t) query.m_message_data[1]) & 0x0F) << 8) + query.m_message_data[2];
      uint16_t param_y = ((((uint16_t) query.m_message_data[3]) & 0x0F) << 8) + query.m_message_data[4];

      current_data.m_param_x0 = m_reversed ? param_y : param_x;
      current_data.m_param_y0 = m_reversed ? param_x : param_y;
    }

    if (1 < current_data.m_touch_count)
    {
      uint16_t param_x = ((((uint16_t) query.m_message_data[7]) & 0x0F) << 8) + query.m_message_data[8];
      uint16_t param_y = ((((uint16_t) query.m_message_data[9]) & 0x0F) << 8) + query.m_message_data[10];

      current_data.m_param_x1 = m_reversed ? param_y : param_x;
      current_data.m_param_y1 = m_reversed ? param_x : param_y;
    }

    registerData (current_data);

    ASSERT_TEST (current_data.m_touch_count < 3);
    m_query_time.leaseFromNow (100);
  }
}

