#ifndef _H735_CLOCK_FREQUENCY_SETUP_H_
#define _H735_CLOCK_FREQUENCY_SETUP_H_

#include "H735ClockFrequency.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockSetup
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735ClockFrequencySetup : public H735ClockFrequency
{
public:
  H735ClockFrequencySetup (uint32_t hse_frequency);

  void setClockFrequency (TypeClockBus, uint32_t max_frequency);
  void setClockFrequencyRefxCk (int pll_index, uint32_t max_frequency);
  void setClockFrequencyVcoxCk (int pll_index, uint32_t max_frequency);

private:
  void setClockFrequencyCoreCk (uint32_t max_frequency);
  void setClockFrequencyHclk (uint32_t max_frequency);
  void setClockFrequencyPclk1 (uint32_t max_frequency);
  void setClockFrequencyPclk2 (uint32_t max_frequency);
  void setClockFrequencyPclk3 (uint32_t max_frequency);
  void setClockFrequencyPclk4 (uint32_t max_frequency);
  void setClockFrequencyPllxZ (int pll_index, int shift_bits, uint32_t max_frequency);

  static uint32_t getPreMask (uint32_t source_frequency, uint32_t target_frequency, uint32_t active_mask);
};

#endif  // _H735_CLOCK_FREQUENCY_SETUP_H_
