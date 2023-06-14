#include "Common.h"
#include "H735PllSingle.h"

#include "PhysicalRccRegisters.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735PllSingle
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735PllSingle::H735PllSingle (TypePllBank pll_bank) :
  m_pll_bank (pll_bank),
  m_div_m (0xFFFFFFFFUL),
  m_div_n (0xFFFFFFFFUL),
  m_div_p (0xFFFFFFFFUL),
  m_div_q (0xFFFFFFFFUL),
  m_div_r (0xFFFFFFFFUL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735PllSingle
// @method:   commit
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735PllSingle::commit (TypePllSource, uint32_t)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735PllSingle
// @method:   setPllClk
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735PllSingle::setPllClk (TypePllClock pll_clock, uint32_t div_m, uint32_t div_n, uint32_t div_clock)
{
  ASSERT_CRITICAL ((0 < div_m) && (div_m < 64));
  ASSERT_CRITICAL ((m_div_m == 0xFFFFFFFFUL) || (m_div_m == div_m));
  ASSERT_CRITICAL (div_n < 512);
  ASSERT_CRITICAL ((m_div_n == 0xFFFFFFFFUL) || (m_div_n == div_n));

  m_div_m = div_m;
  m_div_n = div_n;

  ASSERT_CRITICAL (div_clock < 128);
  switch (pll_clock)
  {
    case PLL_CLOCK_P:
      ASSERT_CRITICAL ((m_div_p == 0xFFFFFFFFUL) || (m_div_p == div_clock));
      m_div_p = div_clock;
      break;

    case PLL_CLOCK_Q:
      ASSERT_CRITICAL ((m_div_q == 0xFFFFFFFFUL) || (m_div_q == div_clock));
      m_div_q = div_clock;
      break;

    case PLL_CLOCK_R:
      ASSERT_CRITICAL ((m_div_r == 0xFFFFFFFFUL) || (m_div_r == div_clock));
      m_div_r = div_clock;
      break;
  }
}

