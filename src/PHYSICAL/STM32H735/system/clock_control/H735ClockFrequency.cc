#include "Common.h"
#include "H735ClockFrequency.h"

#include "PhysicalRccRegisters.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735ClockFrequency::H735ClockFrequency (uint32_t hse_frequency) :
  m_hse_frequency (hse_frequency)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequency (TypeClockBus clock_bus) const
{
  switch (clock_bus)
  {
    case CLOCK_BUS_HSI_CK:
      return getClockFrequencyHsi ();

    case CLOCK_BUS_CSI_CK:
      return getClockFrequencyCsi ();

    case CLOCK_BUS_HSE_CK:
      return getClockFrequencyHse ();

    case CLOCK_BUS_SYS_CK:
      return getClockSystemClock ();

    case CLOCK_BUS_C_CK:
      return getClockFrequencyCoreCk ();

    case CLOCK_BUS_HCLK:
      return getClockFrequencyHclk ();

    case CLOCK_BUS_PCLK1:
      return getClockFrequencyPclk1 ();

    case CLOCK_BUS_PCLK2:
      return getClockFrequencyPclk2 ();

    case CLOCK_BUS_PCLK3:
      return getClockFrequencyPclk3 ();

    case CLOCK_BUS_PCLK4:
      return getClockFrequencyPclk4 ();

    case CLOCK_BUS_PLL_SOURCE:
      return getClockFrequencyPllSource ();

    case CLOCK_BUS_PLL1_REF:
      return getClockFrequencyRefxCk (1);

    case CLOCK_BUS_PLL1_VCO:
      return getClockFrequencyVcoxCk (1);

    case CLOCK_BUS_PLL1_P:
      return getClockFrequencyPllxZ (1, 9);

    case CLOCK_BUS_PLL1_Q:
      return getClockFrequencyPllxZ (1, 16);

    case CLOCK_BUS_PLL1_R:
      return getClockFrequencyPllxZ (1, 24);

    case CLOCK_BUS_PLL2_REF:
      return getClockFrequencyRefxCk (2);

    case CLOCK_BUS_PLL2_VCO:
      return getClockFrequencyVcoxCk (2);

    case CLOCK_BUS_PLL2_P:
      return getClockFrequencyPllxZ (2, 9);

    case CLOCK_BUS_PLL2_Q:
      return getClockFrequencyPllxZ (2, 16);

    case CLOCK_BUS_PLL2_R:
      return getClockFrequencyPllxZ (2, 24);

    case CLOCK_BUS_PLL3_REF:
      return getClockFrequencyRefxCk (3);

    case CLOCK_BUS_PLL3_VCO:
      return getClockFrequencyVcoxCk (3);

    case CLOCK_BUS_PLL3_P:
      return getClockFrequencyPllxZ (3, 9);

    case CLOCK_BUS_PLL3_Q:
      return getClockFrequencyPllxZ (3, 16);

    case CLOCK_BUS_PLL3_R:
      return getClockFrequencyPllxZ (3, 24);

    case CLOCK_BUS_PER_CK:
      return getClockFrequencyPeripherials ();

    case CLOCK_BUS_OCTOSPI_KER_CK:
      return getClockFrequencyOctoSpiKer ();
  }

  ASSERT_CRITICAL (false);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyHsi
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyHsi () const
{
  if ((RCC_REGISTERS.CR & 0x00000004UL) == 0)
  {
    return 0;
  }

  return 64000000;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyCsi
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyCsi () const
{
  if ((RCC_REGISTERS.CR & 0x00000100UL) == 0)
  { 
    return 0;
  }

  return 4000000;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyHse
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyHse () const
{
  if ((RCC_REGISTERS.CR & 0x00020000UL) == 0)
  { 
    return 0;
  }

  return m_hse_frequency;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockSystemClock
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockSystemClock () const
{
  switch (RCC_REGISTERS.CFGR & 0x00000007UL)
  {
    case 0:
      return getClockFrequencyHsi ();

    case 1:
      return getClockFrequencyCsi ();

    case 2:
      return getClockFrequencyHse ();

    case 3:
      // CLOCK_BUS_PLL1_P
      return getClockFrequencyPllxZ (1, 9);

    default:
      break;
  }

  ASSERT_CRITICAL (false);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyPllSource
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyPllSource () const
{
  uint32_t pll_source = RCC_REGISTERS.PLLCKSELR & 0x00000003UL;
  switch (pll_source)
  {
    case 0:
      return getClockFrequencyHsi ();

    case 1:
      return getClockFrequencyCsi ();

    case 2:
      return getClockFrequencyHse ();

    default:
      break;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyRefxCk
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyRefxCk (int pll_index) const
{
  ASSERT_DEV ((0 < pll_index) && (pll_index < 4));

  uint32_t source_frequency = getClockFrequencyPllSource ();
  uint32_t pllckselr = RCC_REGISTERS.PLLCKSELR;

  int div_m_shift = (8 * pll_index) - 4;
  uint32_t div_m = (pllckselr >> div_m_shift) & 0x0000003FUL;

  if (div_m == 0)
  {
    return 0;
  }
  return (source_frequency / div_m);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyVcoxCk
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyVcoxCk (int pll_index) const
{
  uint32_t ref_ck_frequency = getClockFrequencyRefxCk (pll_index);
  uint32_t pllxdivr = (pll_index == 1) ? RCC_REGISTERS.PLL1DIVR :
                      ((pll_index == 2) ? RCC_REGISTERS.PLL2DIVR : RCC_REGISTERS.PLL3DIVR);
  uint32_t div_n = (pllxdivr & 0x000001FFUL) + 1;

  return (ref_ck_frequency * div_n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyPllxZ
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyPllxZ (int pll_index, int shift_bits) const
{
  // Example - pll_index = 1, and shift_bits = 9 (CLOCK_BUS_PLL1_P) yields 16 bits shift, and the value of 0x00010000UL.
  int active_mask = 0x00000001UL << (12 + (shift_bits / 8) + (3 * pll_index));
  if ((RCC_REGISTERS.PLLCFGR & active_mask) == 0)
  {
    return 0;
  }

  uint32_t vco_ck_frequency = getClockFrequencyVcoxCk (pll_index);
  uint32_t pllxdivr = (pll_index == 1) ? RCC_REGISTERS.PLL1DIVR :
                      ((pll_index == 2) ? RCC_REGISTERS.PLL2DIVR : RCC_REGISTERS.PLL3DIVR);
  uint32_t div_p = ((pllxdivr >> shift_bits) & 0x0000007FUL) + 1;

  return (vco_ck_frequency / div_p);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyCoreCk
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyCoreCk () const
{
  uint32_t d1cpre = (RCC_REGISTERS.D1CFGR & 0x00000F00UL) >> 8;

  uint32_t factor = 1;
  if (0x07 <= d1cpre)
  {
    factor = 0x01UL << (d1cpre - 0x07);

    if (0x0C <= d1cpre)
    {
      factor = factor * 2;
    }
  }

  uint32_t system_frequency = getClockSystemClock ();

  return system_frequency / factor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyHclk
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyHclk () const
{
  uint32_t hpre = RCC_REGISTERS.D1CFGR & 0x0000000FUL;

  uint32_t factor = 1;
  if (0x07 <= hpre)
  {
    factor = 0x01UL << (hpre - 0x07);
    if (0x0C <= hpre)
    {
      factor = factor * 2;
    }
  }

  uint32_t core_frequency = getClockFrequencyCoreCk ();

  return (core_frequency / factor);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyPclk1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyPclk1 () const
{
  uint32_t d2ppre1 = (RCC_REGISTERS.D2CFGR & 0x00000070UL) >> 4;
  uint32_t factor = (d2ppre1 < 0x04) ? 1 : (0x01UL << (d2ppre1 - 0x03));

  uint32_t hclk_frequency = getClockFrequencyHclk ();

  return (hclk_frequency / factor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyPclk2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyPclk2 () const
{
  uint32_t d2ppre2 = (RCC_REGISTERS.D2CFGR & 0x00000700UL) >> 8;
  uint32_t factor = (d2ppre2 < 0x04) ? 1 : (0x01UL << (d2ppre2 - 0x03));

  uint32_t hclk_frequency = getClockFrequencyHclk ();

  return (hclk_frequency / factor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyPclk3
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyPclk3 () const
{
  uint32_t d1ppre = (RCC_REGISTERS.D1CFGR & 0x00000070UL) >> 4;
  uint32_t factor = (d1ppre < 0x04) ? 1 : (0x01UL << (d1ppre - 0x03));

  uint32_t hclk_frequency = getClockFrequencyHclk ();

  return (hclk_frequency / factor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyPclk4
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyPclk4 () const
{
  uint32_t d3ppre = (RCC_REGISTERS.D3CFGR & 0x00000070UL) >> 4;
  uint32_t factor = (d3ppre < 0x04) ? 1 : (0x01UL << (d3ppre - 0x03));

  uint32_t hclk_frequency = getClockFrequencyHclk ();

  return (hclk_frequency / factor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyPeripherials
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyPeripherials () const
{
  uint32_t peripherials_source = RCC_REGISTERS.D1CCIPR & 0x30000000UL;
  switch (peripherials_source)
  {
    case 0x00000000UL:
      return getClockFrequencyHsi ();
  
    case 0x10000000UL:
      return getClockFrequencyCsi ();

    case 0x20000000UL:
      return getClockFrequencyHse ();

    default:
      break;
  }

  ASSERT_TEST (false);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
// @method:   getClockFrequencyOctoSpiKer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735ClockFrequency::getClockFrequencyOctoSpiKer () const
{
  uint32_t octospi_sel = RCC_REGISTERS.D1CCIPR & 0x00000030UL;

  switch (octospi_sel)
  {
    case 0x00000000UL:
      return getClockFrequencyHclk ();

    case 0x00000010UL:
      // CLOCK_BUS_PLL1_Q
      return getClockFrequencyPllxZ (1, 16);

    case 0x00000020UL:
      // CLOCK_BUS_PLL2_R
      return getClockFrequencyPllxZ (2, 24);

    case 0x00000030UL:
      return getClockFrequencyPeripherials ();

    default:
      break;
  }

  ASSERT_TEST (false);
  return 0;
}

