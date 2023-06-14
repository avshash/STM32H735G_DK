#include "Common.h"
#include "Disc735ModuleManagerClockControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerClockControl
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Disc735ModuleManagerClockControl::Disc735ModuleManagerClockControl () :
  PhysicalModuleManagerClockControl (25000000UL)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerClockControl
// @method:   setupClockCore
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Disc735ModuleManagerClockControl::setupClockCore ()
{
  // Protect system against high clock while configuring SCGU.
  setClockFrequency (CLOCK_BUS_HCLK, 1000000);

  setClockSource (CLOCK_BUS_SYS_CK, CLOCK_BUS_PLL1_P);

  setClockFrequency (CLOCK_BUS_C_CK, 520000000);
  setClockFrequency (CLOCK_BUS_HCLK, 260000000);
  setClockFrequency (CLOCK_BUS_PCLK1, 130000000);
  setClockFrequency (CLOCK_BUS_PCLK2, 130000000);
  setClockFrequency (CLOCK_BUS_PCLK3, 130000000);
  setClockFrequency (CLOCK_BUS_PCLK4, 130000000);

  ASSERT_TEST (getClockFrequency (CLOCK_BUS_C_CK) == 520000000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerClockControl
// @method:   setupClockPlls
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Disc735ModuleManagerClockControl::setupClockPlls ()
{
  setClockSource (CLOCK_BUS_PLL_SOURCE, CLOCK_BUS_HSE_CK);

  setPllFrequency (1, 5000000, 520000000);
  setPllFrequency (2, 5000000, 400000000);
  setPllFrequency (3, 5000000, 480000000);

  ASSERT_TEST (getClockFrequency (CLOCK_BUS_PLL1_VCO) == 520000000);
  ASSERT_TEST (getClockFrequency (CLOCK_BUS_PLL2_VCO) == 400000000);
  ASSERT_TEST (getClockFrequency (CLOCK_BUS_PLL3_VCO) == 480000000);

  setClockFrequency (CLOCK_BUS_PLL1_P, 520000000);
}

