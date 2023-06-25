#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "TypeInterruptAction.h"
#include "TypeInterruptParam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Interrupt
// @dscr:     A single interrupt pure virtual base.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Interrupt
{
public:
  virtual void handleInterrupt (TypeInterruptAction) = 0;
  virtual int getInterruptParam (TypeInterruptParam) = 0;
  virtual int getIrqNumber (int index) = 0;
};

#endif  // _INTERRUPT_H_
