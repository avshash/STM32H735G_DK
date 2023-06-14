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
  virtual void activateInterrupt (TypeInterruptFamily, TypeInterruptAction) final;
  void open ();
  void setPriority (int irq_number, uint32_t priority);
  void initSysTick ();
};

typedef H735ModuleManagerInterrupts PhysicalModuleManagerInterrupts;

#endif  // _H735_MODULE_MANAGER_INTERRUPTS_H_
