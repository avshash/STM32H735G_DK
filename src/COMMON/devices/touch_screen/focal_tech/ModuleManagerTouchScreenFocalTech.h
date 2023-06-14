#ifndef _MODULE_MANAGER_TOUCH_SCREEN_FOCAL_TECH_H_
#define _MODULE_MANAGER_TOUCH_SCREEN_FOCAL_TECH_H_

#include "ModuleManagerTouchScreen.h"
#include "I2cMaster.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerTouchScreenFocalTech
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerTouchScreenFocalTech : public ModuleManagerTouchScreen
{
protected:
  ModuleManagerTouchScreenFocalTech (ListenerTouchScreen &, I2cMaster &, bool b_reversed);

private:
  void activateGpio ();
  void startModule ();
  void verifyId ();
  void tick ();

  virtual void doAction (TypeModuleAction) final;

  I2cMaster & m_interface;
  const bool m_reversed;                                        // Whether the X-Y axis is reversed (ralative to LCD).
  Timer m_query_time;
};

#endif  // _MODULE_MANAGER_TOUCH_SCREEN_FOCAL_TECH_H_
