#ifndef _DISC735_MODULE_MANAGER_CLOCK_CONTROL_H_
#define _DISC735_MODULE_MANAGER_CLOCK_CONTROL_H_

#include "PhysicalModuleManagerClockControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerClockControl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735ModuleManagerClockControl : public PhysicalModuleManagerClockControl
{
public:
  Disc735ModuleManagerClockControl ();

private:
  virtual void setupClockCore () final;
  virtual void setupClockPlls () final;
};

typedef Disc735ModuleManagerClockControl PlatformModuleManagerClockControl;

#endif  // _DISC735_MODULE_MANAGER_CLOCK_CONTROL_H_

