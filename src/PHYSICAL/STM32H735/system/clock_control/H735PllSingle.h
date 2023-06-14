#ifndef _H735_PLL_SINGLE_H_
#define _H735_PLL_SINGLE_H_

#include "ClockControlTypes.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735PllSingle
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735PllSingle
{
public:
  H735PllSingle (TypePllBank pll_bank);

  void commit (TypePllSource, uint32_t pll_source_freq);
  void setPllClk (TypePllClock, uint32_t div_m, uint32_t div_n, uint32_t div_clock);

private:
  const TypePllBank m_pll_bank;
  uint32_t m_div_m;
  uint32_t m_div_n;
  uint32_t m_div_p;
  uint32_t m_div_q;
  uint32_t m_div_r;
};

#endif  // _H735_PLL_SINGLE_H_
