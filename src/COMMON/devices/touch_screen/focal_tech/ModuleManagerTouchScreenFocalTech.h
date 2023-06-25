#ifndef _MODULE_MANAGER_TOUCH_SCREEN_FOCAL_TECH_H_
#define _MODULE_MANAGER_TOUCH_SCREEN_FOCAL_TECH_H_

#include "ModuleManagerTouchScreen.h"
#include "Timer.h"

class I2cMasterManager;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerTouchScreenFocalTech : public ModuleManagerTouchScreen
{
protected:
  ModuleManagerTouchScreenFocalTech (ListenerTouchScreen &, I2cMasterManager &, bool b_reversed);

private:
  void verifyId ();
  void tick ();
  void handleReply (const uint8_t * reply);

  virtual void doAction (TypeModuleAction) final;

  I2cMasterManager & m_interface;
  const bool m_reversed;                                        // Whether the X-Y axis is reversed (ralative to LCD).
  bool m_active;
  Timer m_query_time;
};

#endif  // _MODULE_MANAGER_TOUCH_SCREEN_FOCAL_TECH_H_
