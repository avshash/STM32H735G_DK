#include "Common.h"
#include "H735ModuleManagerInterrupts.h"

#include "PhysicalSystemControlRegisters.h"
#include "ModuleManagerClockControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerInterrupts
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735ModuleManagerInterrupts::H735ModuleManagerInterrupts ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerInterrupts
// @method:   doAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerInterrupts::doAction (TypeModuleAction action)
{
  switch (action)
  {
    case MODULE_ACTION_TICK:
      break;

    case MODULE_ACTION_OPEN:
      open ();
      break;

    default:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerInterrupts
// @method:   open
// @dscr:     4 bits for group priority (pre-emption), and 4 for subpriority (pending queue priority).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerInterrupts::open ()
{
  // Set interrupt priority grouping field to 3.
  SYSTEM_CONTROL_REGISTERS.AIRCR = 0x05FA0300UL;

  // Set systick reload value to 1 millisecond, based on clock source 64 Mhz.
  SYSTEM_CONTROL_REGISTERS.SYST_RVR = 0x0000FA00UL;
  SYSTEM_CONTROL_REGISTERS.SYST_CVR = 0x0000FA00UL;
  // Enable systick interrupt ('sysTickHandler').
  SYSTEM_CONTROL_REGISTERS.SHPR3 = (SYSTEM_CONTROL_REGISTERS.SHPR3 & 0x00FFFFFFUL) | 0xF0000000UL;
  SYSTEM_CONTROL_REGISTERS.SYST_CSR = 0x00000007UL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerInterrupts
// @method:   enableIrq
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerInterrupts::enableIrq (int irq_number, int priority)
{
  ASSERT_CRITICAL ((0 <= priority) && (priority <= 7));
  ASSERT_CRITICAL ((0 <= irq_number) && (irq_number <= 127));

  // Interrupt priority grouping is 3 bits.
  int ipr_shift = 8 * (irq_number % 4);
  uint32_t priority_shifted = (((uint32_t) priority) << 5) << ipr_shift;
  uint32_t mask = ~(0x000000FFUL << ipr_shift);

  uint32_t active_value = NVIC_REGISTERS.NVIC_IPR[irq_number / 4];

  // Set priority.
  NVIC_REGISTERS.NVIC_IPR[irq_number / 4] = (active_value & mask) | priority_shifted;
  // Enable interrupt.
  int iser_offset = irq_number / 32;
  uint32_t iser_enable_mask = 0x00000001UL << (irq_number % 32);
  NVIC_REGISTERS.NVIC_ISER[iser_offset] = NVIC_REGISTERS.NVIC_ISER[iser_offset] | iser_enable_mask;
}


