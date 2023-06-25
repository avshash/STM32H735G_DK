#ifndef _H735_MODULE_MANAGER_INTERRUPTS_H_
#define _H735_MODULE_MANAGER_INTERRUPTS_H_

#include "ModuleManagerInterrupts.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerInterrupts
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735ModuleManagerInterrupts : public ModuleManagerInterrupts
{
public:
  H735ModuleManagerInterrupts ();

private:
  virtual void doAction (TypeModuleAction) final;
  virtual void enableIrq (int irq_number, int priority) final;

  void open ();
  void initSysTick ();
};

typedef H735ModuleManagerInterrupts PhysicalModuleManagerInterrupts;

#endif  // _H735_MODULE_MANAGER_INTERRUPTS_H_
