#ifndef _H735_CLOCK_FREQUENCY_H_
#define _H735_CLOCK_FREQUENCY_H_

#include "TypeClockBus.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockFrequency
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735ClockFrequency
{
public:
  uint32_t getClockFrequency (TypeClockBus) const;
  uint32_t getClockFrequencyHsi () const;

protected:
  H735ClockFrequency (uint32_t hse_frequency);

  uint32_t getClockFrequencyCsi () const;
  uint32_t getClockFrequencyHse () const;
  uint32_t getClockSystemClock () const;
  uint32_t getClockFrequencyPllSource () const;
  uint32_t getClockFrequencyRefxCk (int pll_index) const;
  uint32_t getClockFrequencyVcoxCk (int pll_index) const;
  uint32_t getClockFrequencyPllxZ (int pll_index, int shift_bits) const;
  uint32_t getClockFrequencyCoreCk () const;
  uint32_t getClockFrequencyHclk () const;
  uint32_t getClockFrequencyPclk1 () const;
  uint32_t getClockFrequencyPclk2 () const;
  uint32_t getClockFrequencyPclk3 () const;
  uint32_t getClockFrequencyPclk4 () const;
  uint32_t getClockFrequencyPeripherials () const;
  uint32_t getClockFrequencyOctoSpiKer () const;

  const uint32_t m_hse_frequency;
};

#endif  // _H735_CLOCK_FREQUENCY_H_
