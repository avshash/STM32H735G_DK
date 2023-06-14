#include "Common.h"
#include "ModuleManagerTouchScreen.h"

#include "TextSingleLine.h"
#include "ListenerTouchScreen.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreen
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerTouchScreen::ModuleManagerTouchScreen (ListenerTouchScreen & listener) :
  ModuleManager (MODULE_MANAGER_TOUCH_SCREEN),
  m_listener (listener)
{
  memset (&m_reported_state, 0, sizeof (m_reported_state));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreen
// @method:   registerData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerTouchScreen::registerData (const TouchData & active_data)
{
  if (memcmp (&m_reported_state, &active_data, sizeof (active_data)) != 0)
  {
    m_listener.registerAction (m_reported_state, active_data);
    memcpy (&m_reported_state, &active_data, sizeof (active_data));
  }
}

