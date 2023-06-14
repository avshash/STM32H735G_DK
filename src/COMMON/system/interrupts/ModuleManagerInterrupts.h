#ifndef _MODULE_MANAGER_INTERRUPTS_H_
#define _MODULE_MANAGER_INTERRUPTS_H_

#include "ModuleManager.h"
#include "TypeInterruptFamily.h"
#include "TypeInterruptAction.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerInterrupts
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerInterrupts : public ModuleManager
{
public:
  ModuleManagerInterrupts ();

  virtual void activateInterrupt (TypeInterruptFamily, TypeInterruptAction) = 0;
};

#endif  // _MODULE_MANAGER_INTERRUPTS_H_
