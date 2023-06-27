#include "Common.h"
#include "ModuleManagerInterrupts.h"

#include "Interrupt.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerInterrupts
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerInterrupts::ModuleManagerInterrupts () :
  ModuleManager (MODULE_MANAGER_INTERRUPTS)
{
  memset (m_handlers, 0, sizeof (m_handlers));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerInterrupts
// @method:   handleInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerInterrupts::handleInterrupt (TypeInterruptHandler family, TypeInterruptAction action)
{
  ASSERT_TEST (m_handlers[family] != NULL);
  m_handlers[family]->handleInterrupt (action);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerInterrupts
// @method:   registerInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerInterrupts::registerInterrupt (Interrupt & handler)
{
  TypeInterruptHandler family = (TypeInterruptHandler) handler.getInterruptParam (INTERRUPT_PARAM_HANDLER_GROUP);
  int priority = handler.getInterruptParam (INTERRUPT_PARAM_IRQ_PRIORITY);

  ASSERT_TEST (m_handlers[family] == NULL);
  ASSERT_TEST ((0 <= family) && (family < INTERRUPT_HANDLER_ACTIVE_COUNT));
  ASSERT_TEST ((0 <= priority) && (priority <= 7));

  m_handlers[family] = &handler;

  int irq_count = handler.getInterruptParam (INTERRUPT_PARAM_IRQ_ENTRIES_COUNT);
  for (int irq_index = 0; irq_index < irq_count; irq_index++)
  {
    int irq_number = handler.getIrqNumber (irq_index);
    enableIrq (irq_number, priority);
  }
}

