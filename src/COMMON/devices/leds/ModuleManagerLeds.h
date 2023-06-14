#ifndef _MODULE_MANAGER_LEDS_H_
#define _MODULE_MANAGER_LEDS_H_

#include "ModuleManager.h"
#include "TypeLedColor.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLeds
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerLeds : public ModuleManager
{
public:
  virtual void setLed (TypeLedColor, bool b_on) = 0;

protected:
  ModuleManagerLeds ();

  virtual void doAction (TypeModuleAction) final;
  virtual void activateGpio () = 0;
};

#endif  // _MODULE_MANAGER_LEDS_H_
