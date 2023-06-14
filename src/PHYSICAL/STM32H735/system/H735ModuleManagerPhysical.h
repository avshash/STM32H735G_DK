#ifndef _H735_MODULE_MANAGER_PHYSICAL_H_
#define _H735_MODULE_MANAGER_PHYSICAL_H_

#include "ModuleManagerPhysical.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerPhysical
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735ModuleManagerPhysical : public ModuleManagerPhysical
{
public:
  H735ModuleManagerPhysical ();

private:
  virtual void doAction (TypeModuleAction) final;

  void systemInit ();
  void systemStart ();
  void enableCache ();
  void setPower ();
};

typedef H735ModuleManagerPhysical PhysicalModuleManagerPhysical;

#endif  // _H735_MODULE_MANAGER_PHYSICAL_H_
