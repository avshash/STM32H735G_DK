#include "Common.h"
#include "H735ModuleManagerClockControl.h"

#include "PhysicalRccRegisters.h"
#include "PhysicalSystemControlRegisters.h"
#include "H735ClockFrequency.h"
#include "SystemTime.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735ModuleManagerClockControl::H735ModuleManagerClockControl (uint32_t hse_frequency) :
  ModuleManagerClockControl (),
  m_clock_setup (hse_frequency)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   disablePlls
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::disablePlls ()
{
  RCC_REGISTERS.PLLCFGR = 0;
  RCC_REGISTERS.PLLCKSELR = 0x00000003UL;
  RCC_REGISTERS.PLL1DIVR = 0x00000003UL;
  RCC_REGISTERS.PLL1FRACR = 0;
  RCC_REGISTERS.PLL2DIVR = 0x00000003UL;
  RCC_REGISTERS.PLL2FRACR = 0;
  RCC_REGISTERS.PLL3DIVR = 0x00000003UL;
  RCC_REGISTERS.PLL3FRACR = 0;

  RCC_REGISTERS.CR = RCC_REGISTERS.CR & 0xC0FFFFFFUL;
  while ((RCC_REGISTERS.CR & 0x3F000000UL) != 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   activatePlls
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::activatePlls ()
{
  uint32_t pllcfgr = RCC_REGISTERS.PLLCFGR;
  if ((pllcfgr & 0x00070000UL) != 0)
  {
    activatePll (1);
  }
  if ((pllcfgr & 0x00380000UL) != 0)
  {
    activatePll (2);
  }
  if ((pllcfgr & 0x01C00000UL) != 0)
  {
    activatePll (3);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   activatePll
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::activatePll (int index)
{
  ASSERT_TEST ((0 < index) && (index < 4));

  TypeClockBus ref_ck = (index == 1) ? CLOCK_BUS_PLL1_REF : ((index == 2) ? CLOCK_BUS_PLL2_REF : CLOCK_BUS_PLL3_REF);
  TypeClockBus vco_ck = (index == 1) ? CLOCK_BUS_PLL1_VCO : ((index == 2) ? CLOCK_BUS_PLL2_VCO : CLOCK_BUS_PLL3_VCO);

  uint32_t ref_frequency = getClockFrequency (ref_ck);
  uint32_t vco_frequency = getClockFrequency (vco_ck);
  ASSERT_TEST ((1000000 <= ref_frequency) && (ref_frequency <= 16000000));

  uint32_t pllcfgr = RCC_REGISTERS.PLLCFGR;
  int shift = 4 * (index - 1);

  if (ref_frequency <= 2000000)
  {
    RCC_REGISTERS.PLLCFGR = pllcfgr | (0x00000002UL << shift);
    ASSERT_TEST ((150000000 <= vco_frequency) && (vco_frequency <= 420000000));
  }
  else
  {
    ASSERT_TEST ((192000000 <= vco_frequency) && (vco_frequency <= 836000000));
    if (ref_frequency <= 4000000)
    {
      RCC_REGISTERS.PLLCFGR = pllcfgr | (0x00000004UL << shift);
    }
    else if (ref_frequency <= 8000000)
    {
      RCC_REGISTERS.PLLCFGR = pllcfgr | (0x00000008UL << shift);
    }
    else
    {
      RCC_REGISTERS.PLLCFGR = pllcfgr | (0x0000000CUL << shift);
    }
  }

  uint32_t cr_mask = 0x00000001UL << (22 + (index * 2));
  RCC_REGISTERS.CR = RCC_REGISTERS.CR | cr_mask;
  cr_mask = cr_mask << 1;
  while ((RCC_REGISTERS.CR & cr_mask) != 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   doAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::doAction (TypeModuleAction action)
{
  switch (action)
  {
    case MODULE_ACTION_TICK:
      break;

    case MODULE_ACTION_OPEN:
      disablePlls ();
      // Make sure that div_m, and div_n are setup before the rest of the plls.
      setupClockPlls ();
      break;

    case MODULE_ACTION_START_PLL_CLOCKS:
      activatePlls ();
      setupClockCore ();
      break;

    default:
       break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   setClockSource
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::setClockSource (TypeClockBus clock, TypeClockBus source)
{
  enableSource (source);

  switch (clock)
  {
    case CLOCK_BUS_SYS_CK:
      setSysCkSource (source);
      break;

    case CLOCK_BUS_PLL_SOURCE:
      setPllSource (source);
      break;

    default:
      ASSERT_TEST (false);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   setPllFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::setPllFrequency (int pll_index, uint32_t ref_max_frequency, uint32_t vco_max_frequency)
{
  m_clock_setup.setClockFrequencyRefxCk (pll_index, ref_max_frequency);
  m_clock_setup.setClockFrequencyVcoxCk (pll_index, vco_max_frequency);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   activateGpioClock
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::activateGpioClock (TypeGpioBank gpio_bank)
{
  ASSERT_DEV ((GPIO_BANK_K - GPIO_BANK_A) == 10);

  uint32_t shift_bits = (gpio_bank - GPIO_BANK_A);
  uint32_t enable_mask = 0x00000001UL << shift_bits;

  // Do not support GPIO_BANK_I.
  ASSERT_TEST (shift_bits != 8);
  ASSERT_CRITICAL (shift_bits < 11);

  RCC_REGISTERS.AHB4ENR = RCC_REGISTERS.AHB4ENR | enable_mask;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   enableSource
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::enableSource (TypeClockBus source)
{
  switch (source)
  {
    case CLOCK_BUS_HSI_CK:
      RCC_REGISTERS.CR = RCC_REGISTERS.CR | 0x00000001UL;
      while ((RCC_REGISTERS.CR & 0x00000004UL) == 0);
      break;

    case CLOCK_BUS_CSI_CK:
      RCC_REGISTERS.CR = RCC_REGISTERS.CR | 0x00000080UL;
      while ((RCC_REGISTERS.CR & 0x00000100UL) == 0);
      break;

    case CLOCK_BUS_HSE_CK:
      RCC_REGISTERS.CR = RCC_REGISTERS.CR | 0x00010000UL;
      while ((RCC_REGISTERS.CR & 0x00020000UL) == 0);
      break;

    case CLOCK_BUS_PLL1_P:
      // pll1_p_ck is enabled only by properly setting up the pll1 params.
      break;

    default:
      ASSERT_TEST (false);
      break;
  }

  ASSERT_TEST (0 < m_clock_setup.getClockFrequency (source));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   setSysCkSource
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::setSysCkSource (TypeClockBus source)
{
  uint32_t source_value = 0;

  switch (source)
  {
    case CLOCK_BUS_HSI_CK:
      source_value = 0x00000000UL;
      break;

    case CLOCK_BUS_CSI_CK:
      source_value = 0x00000001UL;
      break;

    case CLOCK_BUS_HSE_CK:
      source_value = 0x00000002UL;
      break;

    case CLOCK_BUS_PLL1_P:
      ASSERT_TEST (getClockFrequency (CLOCK_BUS_PLL1_P) <= 550000000);
      // Verify that the new hclk frequency is still valid (mulitply be the old source divied by the new source).
      ASSERT_TEST ((getClockFrequency (CLOCK_BUS_PLL1_P) / getClockFrequency (CLOCK_BUS_SYS_CK) *
                    getClockFrequency (CLOCK_BUS_HCLK)) <= 275000000);

      source_value = 0x00000003UL;
      break;

    default:
      ASSERT_TEST (false);
      break;
  }

  // Set the requested clock active source.
  RCC_REGISTERS.CFGR = (RCC_REGISTERS.CFGR & 0xFFFFFFC0UL) | source_value;

  // Wait for the actual clock source to reach the desired value.
  uint32_t active_value = source_value << 3;
  while ((RCC_REGISTERS.CFGR & 0x000000038UL) != active_value);

  setupSysTick ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   setPllSource
// @note:     Disables all Plls (sets DIVM to 0).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::setPllSource (TypeClockBus source)
{
  disablePlls ();

  switch (source)
  {
    case CLOCK_BUS_HSI_CK:
      RCC_REGISTERS.PLLCKSELR = 0x00000000UL;
      break;

    case CLOCK_BUS_CSI_CK:
      RCC_REGISTERS.PLLCKSELR = 0x00000001UL;
      break;

    case CLOCK_BUS_HSE_CK:
      RCC_REGISTERS.PLLCKSELR = 0x00000002UL;
      break;

    default:
      ASSERT_TEST (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   getClockFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ModuleManagerClockControl::getClockFrequency (TypeClockBus clock_type) const
{
  return m_clock_setup.getClockFrequency (clock_type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   setClockFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::setClockFrequency (TypeClockBus clock_type, uint32_t max_frequency)
{
  m_clock_setup.setClockFrequency (clock_type, max_frequency);

  if (clock_type == CLOCK_BUS_C_CK)
  {
    setupSysTick ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   setupSysTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::setupSysTick ()
{
  uint32_t system_frequency = getClockFrequency (CLOCK_BUS_C_CK);
  ASSERT_CRITICAL (1000000 <= system_frequency);

  uint32_t ticks_per_millisecond = system_frequency / 1000;
  SYSTEM_CONTROL_REGISTERS.SYST_RVR = ticks_per_millisecond - 1;
  SYSTEM_CONTROL_REGISTERS.SYST_CVR = ticks_per_millisecond - 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @method:   waitMicro
// @note:     'SYST_CVR' is counting down to 0 from the initial 'SYST_RVR' value.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ModuleManagerClockControl::waitMicro (uint32_t micro_seconds) const
{
  uint32_t initial_systick = SystemTime::getSystemMilliTime ();
  uint32_t initial_cvr = SYSTEM_CONTROL_REGISTERS.SYST_CVR & 0x00FFFFFFUL;
  uint32_t cur_systick = SystemTime::getSystemMilliTime ();
  uint32_t cur_cvr = SYSTEM_CONTROL_REGISTERS.SYST_CVR & 0x00FFFFFFUL;

  if (initial_systick < cur_systick)
  {
    // Advanced 1 tick. 'cur_cvr' is valid for 'cur_systick', so we can use minimum as best effort.
    initial_cvr = MAX (initial_cvr, cur_cvr);
    initial_systick++;
  }

  // One tick is 1000 microseconds. The SYST_RVR, and SYST_CVR registers are limited to 0x00FFFFFFUL.
  // For overflow reasons, we must divide the registers' value by 4 before multiplying by 1000.
  const uint32_t syst_rvr = SYSTEM_CONTROL_REGISTERS.SYST_RVR & 0x00FFFFFFUL;
  uint32_t initial_micro = 1000 - (1000 * (initial_cvr / 4)) / (syst_rvr / 4);
  uint32_t target_systick = initial_systick + ((micro_seconds + initial_micro) / 1000);
  uint32_t target_micro = (initial_micro + micro_seconds) % 1000;
  uint32_t target_cvr = syst_rvr - ((target_micro * (syst_rvr / 4)) / 250);

  // Overflow protection.
  ASSERT_CRITICAL (initial_systick <= target_systick);

  // Wait milliseoncds using the sysTick interrupt.
  while (cur_systick <= target_systick)
  {
    if ((target_systick == cur_systick) && (SYSTEM_CONTROL_REGISTERS.SYST_CVR <= target_cvr))
    {
      break;
    }
    cur_systick = SystemTime::getSystemMilliTime ();
  }
}

