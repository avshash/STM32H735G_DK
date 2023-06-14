#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include "TypeModuleManager.h"
#include "TypeModuleAction.h"

class ModuleManagerMain;
class ModuleManagerInterrupts;
class ModuleManagerClockControl;
class ModuleManagerLeds;
class ModuleManagerLcd;
class ModuleManagerFlash;
class ModuleManagerSdram;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManager
{
public:
  virtual void doAction (TypeModuleAction) = 0;

  static bool testModuleActive (TypeModuleManager);
  static ModuleManagerInterrupts & getInterrupts ();
  static ModuleManagerLcd & getLcd ();
  static ModuleManagerLeds & getLeds ();
  static ModuleManagerFlash & getFlash ();
  static ModuleManagerSdram & getSdram ();
  static ModuleManagerClockControl & getClockControl ();

protected:
  ModuleManager (TypeModuleManager);
};

#endif  // _MODULE_MANAGER_H_
