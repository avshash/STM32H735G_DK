#include "Common.h"
#include "H735ClockFrequencySetup.h"

#include "PhysicalRccRegisters.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735ClockFrequencySetup::H735ClockFrequencySetup (uint32_t hse_frequency) :
  H735ClockFrequency (hse_frequency)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequency (TypeClockBus clock_bus, uint32_t max_frequency)
{
  switch (clock_bus)
  {
    case CLOCK_BUS_C_CK:
      setClockFrequencyCoreCk (max_frequency);
      break;

    case CLOCK_BUS_HCLK:
      setClockFrequencyHclk (max_frequency);
      break;

    case CLOCK_BUS_PCLK1:
      setClockFrequencyPclk1 (max_frequency);
      break;

    case CLOCK_BUS_PCLK2:
      setClockFrequencyPclk2 (max_frequency);
      break;

    case CLOCK_BUS_PCLK3:
      setClockFrequencyPclk3 (max_frequency);
      break;

    case CLOCK_BUS_PCLK4:
      setClockFrequencyPclk4 (max_frequency);
      break;

    case CLOCK_BUS_PLL1_REF:
      ASSERT_TEST (false);
      break;

    case CLOCK_BUS_PLL1_VCO:
      ASSERT_TEST (false);
      break;

    case CLOCK_BUS_PLL1_P:
      setClockFrequencyPllxZ (1, 9, max_frequency);
      break;

    case CLOCK_BUS_PLL1_Q:
      setClockFrequencyPllxZ (1, 16, max_frequency);
      break;

    case CLOCK_BUS_PLL1_R:
      setClockFrequencyPllxZ (1, 24, max_frequency);
      break;

    case CLOCK_BUS_PLL2_REF:
      ASSERT_TEST (false);
      break;

    case CLOCK_BUS_PLL2_VCO:
      ASSERT_TEST (false);
      break;

    case CLOCK_BUS_PLL2_P:
      setClockFrequencyPllxZ (2, 9, max_frequency);
      break;

    case CLOCK_BUS_PLL2_Q:
      setClockFrequencyPllxZ (2, 16, max_frequency);
      break;

    case CLOCK_BUS_PLL2_R:
      setClockFrequencyPllxZ (2, 24, max_frequency);
      break;

    case CLOCK_BUS_PLL3_REF:
      ASSERT_TEST (false);
      break;

    case CLOCK_BUS_PLL3_VCO:
      ASSERT_TEST (false);
      break;

    case CLOCK_BUS_PLL3_P:
      setClockFrequencyPllxZ (3, 9, max_frequency);
      break;

    case CLOCK_BUS_PLL3_Q:
      setClockFrequencyPllxZ (3, 16, max_frequency);
      break;

    case CLOCK_BUS_PLL3_R:
      setClockFrequencyPllxZ (3, 24, max_frequency);
      break;

    case CLOCK_BUS_HSI_CK:
    case CLOCK_BUS_CSI_CK:
    case CLOCK_BUS_HSE_CK:
    case CLOCK_BUS_SYS_CK:
    case CLOCK_BUS_PLL_SOURCE:
    case CLOCK_BUS_PER_CK:
    case CLOCK_BUS_OCTOSPI_KER_CK:
      ASSERT_CRITICAL (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequencyRefxCk
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequencyRefxCk (int pll_index, uint32_t max_frequency)
{
  ASSERT_DEV ((0 < pll_index) && (pll_index < 4));

  uint32_t source_frequency = getClockFrequencyPllSource ();
  ASSERT_CRITICAL (0 < source_frequency);

  uint32_t factor = ((source_frequency + max_frequency - 1) / max_frequency);
  ASSERT_CRITICAL (factor < 0x40);

  int div_m_shift = (8 * pll_index) - 4;
  uint32_t pllckselr_base = RCC_REGISTERS.PLLCKSELR & (~(0x3FUL << div_m_shift));

  RCC_REGISTERS.PLLCKSELR = pllckselr_base | (factor << div_m_shift);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequencyVcoxCk
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequencyVcoxCk (int pll_index, uint32_t max_frequency)
{
  // 'getClockFrequencyRefxCk' verifies that 'pll_index' is valid.
  uint32_t source_frequency = getClockFrequencyRefxCk (pll_index);
  ASSERT_CRITICAL (0 < source_frequency);

  uint32_t factor = max_frequency / source_frequency;
  ASSERT_CRITICAL ((3 < factor) && (factor <= 0x00000200UL));

  volatile uint32_t & pllxdivr = (pll_index == 1) ? RCC_REGISTERS.PLL1DIVR :
                                 ((pll_index == 2) ? RCC_REGISTERS.PLL2DIVR : RCC_REGISTERS.PLL3DIVR);

  pllxdivr = (pllxdivr & 0xFFFFFE00UL) | (factor - 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequencyPllxZ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequencyPllxZ (int pll_index, int shift_bits, uint32_t max_frequency)
{
  // 'getClockFrequencyVcoxCk' verifies that 'pll_index' is valid.
  uint32_t source_frequency = getClockFrequencyVcoxCk (pll_index);
  ASSERT_CRITICAL (0 < source_frequency);

  uint32_t factor = (source_frequency + max_frequency - 1) / max_frequency;
  factor = (factor == 0) ? 0 : (factor - 1);
  ASSERT_CRITICAL (factor < 0x00000100UL);

  volatile uint32_t & pllxdivr = (pll_index == 1) ? RCC_REGISTERS.PLL1DIVR :
                                 ((pll_index == 2) ? RCC_REGISTERS.PLL2DIVR : RCC_REGISTERS.PLL3DIVR);

  uint32_t pllxdivr_base = pllxdivr & ~(0x7FUL << shift_bits);
  pllxdivr = pllxdivr_base | (factor << shift_bits);

  // Example - pll_index = 1, and shift_bits = 9 (CLOCK_BUS_PLL1_P) yields 16 bits shift, and the value of 0x00010000UL.
  int active_mask = 0x00000001UL << (12 + (shift_bits / 8) + (3 * pll_index));
  RCC_REGISTERS.PLLCFGR = RCC_REGISTERS.PLLCFGR | active_mask;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequencyCoreCk
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequencyCoreCk (uint32_t max_frequency)
{
  ASSERT_CRITICAL (max_frequency <= 550000000);
  uint32_t d1cpre_value = getPreMask (getClockSystemClock (), max_frequency, 0x08);

  uint32_t d1cfgr_base = (RCC_REGISTERS.D1CFGR & 0xFFFFF0FFUL);
  RCC_REGISTERS.D1CFGR = d1cfgr_base | (d1cpre_value << 8);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequencyHclk
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequencyHclk (uint32_t max_frequency)
{
  ASSERT_CRITICAL (max_frequency <= 275000000);

  uint32_t hpre_value = getPreMask (getClockFrequencyCoreCk (), max_frequency, 0x08);

  uint32_t d1cfgr_base = (RCC_REGISTERS.D1CFGR & 0xFFFFFFF0UL);
  RCC_REGISTERS.D1CFGR = d1cfgr_base | hpre_value;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequencyPclk1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequencyPclk1 (uint32_t max_frequency)
{
  uint32_t d2ppre1_value = getPreMask (getClockFrequencyHclk (), max_frequency, 0x04);

  uint32_t d2cfgr_base = (RCC_REGISTERS.D2CFGR & 0xFFFFFF8FUL);
  RCC_REGISTERS.D2CFGR = d2cfgr_base | (d2ppre1_value << 4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequencyPclk2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequencyPclk2 (uint32_t max_frequency)
{
  uint32_t d2ppre2_value = getPreMask (getClockFrequencyHclk (), max_frequency, 0x04);

  uint32_t d2cfgr_base = (RCC_REGISTERS.D2CFGR & 0xFFFFFF8FUL);
  RCC_REGISTERS.D2CFGR = d2cfgr_base | (d2ppre2_value << 8);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequencyPclk3
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequencyPclk3 (uint32_t max_frequency)
{
  uint32_t d1ppre_value = getPreMask (getClockFrequencyHclk (), max_frequency, 0x04);

  uint32_t d1cfgr_base = (RCC_REGISTERS.D1CFGR & 0xFFFFFF8FUL);
  RCC_REGISTERS.D1CFGR = d1cfgr_base | (d1ppre_value << 4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   setClockFrequencyPclk4
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735ClockFrequencySetup::setClockFrequencyPclk4 (uint32_t max_frequency)
{
  uint32_t d3ppre_value = getPreMask (getClockFrequencyHclk (), max_frequency, 0x04);

  uint32_t d3cfgr_base = (RCC_REGISTERS.D3CFGR & 0xFFFFFF8FUL);
  RCC_REGISTERS.D3CFGR = d3cfgr_base | (d3ppre_value << 4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequencySetup
// @method:   getPreMask
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequencySetup::getPreMask (uint32_t source_frequency, uint32_t target_frequency, uint32_t active_mask)
{
  ASSERT_TEST ((0 < target_frequency) && (0 < source_frequency));;

  uint32_t shift_bits = 0;
  while (target_frequency < source_frequency)
  {
    source_frequency = source_frequency / 2;
    shift_bits++;
  }

  uint32_t ret_val = 0;
  if (0 < shift_bits)
  {
    uint32_t shift_mask = (4 < shift_bits) ? (shift_bits - 2) : (shift_bits - 1);
    ASSERT_CRITICAL (shift_mask < active_mask);

    ret_val = active_mask | shift_mask;
  }

  return ret_val;
}

