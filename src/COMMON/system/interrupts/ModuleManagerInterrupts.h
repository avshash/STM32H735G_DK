#ifndef _MODULE_MANAGER_INTERRUPTS_H_
#define _MODULE_MANAGER_INTERRUPTS_H_

#include "ModuleManager.h"
#include "TypeInterruptHandler.h"
#include "TypeInterruptAction.h"

class Interrupt;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerInterrupts
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerInterrupts : public ModuleManager
{
public:
  ModuleManagerInterrupts ();

  void registerInterrupt (Interrupt &);
  void handleInterrupt (TypeInterruptHandler, TypeInterruptAction);

private:
  virtual void enableIrq (int irq_number, int priority) = 0;

  Interrupt * m_handlers[INTERRUPT_HANDLER_ACTIVE_COUNT];
};

#endif  // _MODULE_MANAGER_INTERRUPTS_H_
