#ifndef _MODULE_MANAGER_CLOCK_CONTROL_H_
#define _MODULE_MANAGER_CLOCK_CONTROL_H_

#include "ModuleManager.h"
#include "TypeClockParam.h"
#include "TypeClockBus.h"
#include "TypeGpioBank.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerClockControl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerClockControl : public ModuleManager
{
public:
  ModuleManagerClockControl ();

  virtual void setClockFrequency (TypeClockBus, uint32_t max_frequency) = 0;
  virtual void activateGpioClock (TypeGpioBank) = 0;
  virtual uint32_t getClockFrequency (TypeClockBus) const = 0;
  virtual void waitMicro (uint32_t micro_seconds) const = 0;
};

#endif  // _MODULE_MANAGER_CLOCK_CONTROL_H_
