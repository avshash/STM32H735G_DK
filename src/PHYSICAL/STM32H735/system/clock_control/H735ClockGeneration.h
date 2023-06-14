#ifndef _H735_CLOCK_GENERATION_H_
#define _H735_CLOCK_GENERATION_H_

#include "ClockControlTypes.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ClockGeneration
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735ClockGeneration
{
public:
  H735ClockGeneration ();

  void activate ();
  void setParam (TypeClockParam, uint32_t val);
  void setFrequency (TypeClockBusFrequency, uint32_t value);

private:
  uint32_t m_params[CLOCK_PARAM_COUNT];
  uint32_t m_frequencies[CLOCK_FREQUENCY_BUS_COUNT];
};

#endif  // _H735_CLOCK_GENERATION_H_
