#include "Common.h"
#include "H735ModuleManagerPhysical.h"

#include "PhysicalSystemControlRegisters.h"
#include "PhysicalPwrRegisters.h"
#include "PhysicalFmcRegisters.h"
#include "PhysicalFlashRegisters.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerPhysical
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735ModuleManagerPhysical::H735ModuleManagerPhysical () :
  ModuleManagerPhysical ()
{
  systemInit ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerPhysical
// @method:   doAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerPhysical::doAction (TypeModuleAction action)
{
  switch (action)
  {
    case MODULE_ACTION_TICK:
      break;

    case MODULE_ACTION_START:
      systemStart ();
      break;

    default:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerPhysical
// @method:   enableCache
// @dscr:     Enable cache as part of the system initialization. Invoked when interrupts are disabled.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerPhysical::enableCache ()
{
  PhysicalSystemControlRegisters & system_control_registers = SYSTEM_CONTROL_REGISTERS;

  uint32_t ccr = system_control_registers.CCR;
  if ((ccr & 0x00030000UL) == 0x00030000UL)
  {
    return;
  }

  // Invalidate instruction cache.
  system_control_registers.ICIALLU = 0;

  // Set access to data cache.
  system_control_registers.CSSELR = 0;

  uint32_t num_sets = 1 + ((system_control_registers.CCSIDR & 0x0FFFE000UL) >> 13);
  uint32_t num_ways = 1 + ((system_control_registers.CCSIDR & 0x00001FF8UL) >> 3);

  while (4 < num_ways);
  while (0x100 < num_sets);
  ASSERT_TEST (num_sets <= 0x100);
  ASSERT_TEST (num_ways <= 4);

  // Invalidate L1 data cache.
  for (uint32_t set = 0; set < num_sets; set++)
  {
    uint32_t set_mask = set << 5;
    for (uint32_t way = 0; way < num_ways; way++)
    {
      system_control_registers.DCISW = set_mask | (way << 30);
    }
  }

  // Enable instruction cache and memory cache.
  system_control_registers.CCR = system_control_registers.CCR | 0x00030000UL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerPhysical
// @method:   systemInit
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerPhysical::systemInit ()
{
  // Enable full access to both coprocessors.
  SYSTEM_CONTROL_REGISTERS.CPACR = 0x00F00000UL;

  // Sets the Vector table to point to flash base.
  SYSTEM_CONTROL_REGISTERS.VTOR = 0x08000000UL;

  // Disable bank 1 memory (default is enabled).
  FMC_REGISTERS.BCR1 = 0x000030D2UL;

  enableCache ();
  setPower ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerPhysical
// @method:   setPower
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerPhysical::setPower ()
{
  // Set power to VOS0 (boosted performence).
  PWR_REGISTERS.D3CR = 0;

  while (((PWR_REGISTERS.D3CR) & 0x00002000UL) == 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerPhysical
// @method:   systemStart
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerPhysical::systemStart ()
{
  // Set flash latency to lowest safe value (for this family).
  // Executed only after RCC initialization.
  FLASH_REGISTERS.ACR = 0x33;
  while (FLASH_REGISTERS.ACR != 0x33);
}

