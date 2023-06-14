#ifndef _MODULE_MANAGER_TOUCH_SCREEN_H_
#define _MODULE_MANAGER_TOUCH_SCREEN_H_

#include "ModuleManager.h"
#include "TouchData.h"

class ListenerTouchScreen;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerTouchScreen : public ModuleManager
{
protected:
  ModuleManagerTouchScreen (ListenerTouchScreen &);

  void registerData (const TouchData & active_data);

private:
  ListenerTouchScreen & m_listener;
  TouchData m_reported_state;
};

#endif  // _MODULE_MANAGER_TOUCH_SCREEN_H_
