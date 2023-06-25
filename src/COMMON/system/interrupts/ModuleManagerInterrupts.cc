#include "Common.h"
#include "ModuleManagerInterrupts.h"

#include "Interrupt.h"
#include "CriticalSection.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerInterrupts
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerInterrupts::ModuleManagerInterrupts () :
  ModuleManager (MODULE_MANAGER_INTERRUPTS),
  m_critical_section (NULL)
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
  if (m_critical_section != NULL)
  {
    // We must cover the case where we leave a critical section, but interrupts are not fully enabled yet.
    // In such a case, a low priority interrupt might be reached before a high priority.
    // (This happends when enabling interrupts is not implemented as an atomic action).
    m_critical_section->handleInterrupt ();
  }

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerInterrupts
// @method:   criticalSection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerInterrupts::criticalSection (CriticalSection * critical_section)
{
  if (critical_section != NULL)
  {
    critical_section->setPrevActive (m_critical_section);
    m_critical_section = critical_section;
  }
  else
  {
    m_critical_section = m_critical_section->getPrevActive ();
  }
}

