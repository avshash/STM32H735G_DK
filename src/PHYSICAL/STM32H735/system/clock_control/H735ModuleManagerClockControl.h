#ifndef _H735_MODULE_MANAGER_CLOCK_CONTROL_H_
#define _H735_MODULE_MANAGER_CLOCK_CONTROL_H_

#include "ModuleManagerClockControl.h"
#include "H735ClockFrequencySetup.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735ModuleManagerClockControl
// @dscr:     Registers and system behavior documented by 'RM0468' sction 8 (Reset and Clock Control - RCC).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735ModuleManagerClockControl : public ModuleManagerClockControl
{
protected:
  H735ModuleManagerClockControl (uint32_t hse_frequency);

  virtual void setClockFrequency (TypeClockBus, uint32_t max_frequency) final;
  virtual uint32_t getClockFrequency (TypeClockBus) const final;
  void setClockSource (TypeClockBus clock, TypeClockBus source);
  void setPllFrequency (int pll_index, uint32_t ref_max_frequency, uint32_t vco_max_frequency);

private:
  virtual void doAction (TypeModuleAction) final;
  virtual void activateGpioClock (TypeGpioBank) final;
  virtual void waitMicro (uint32_t micro_seconds) const final;
  void disablePlls ();
  void activatePlls ();
  void activatePll (int index);
  void enableSource (TypeClockBus);
  void setSysCkSource (TypeClockBus);
  void setPllSource (TypeClockBus);
  void setupSysTick ();

  virtual void setupClockCore () = 0;
  virtual void setupClockPlls () = 0;

  H735ClockFrequencySetup m_clock_setup;
};

typedef H735ModuleManagerClockControl PhysicalModuleManagerClockControl;

#endif  // _H735_MODULE_MANAGER_CLOCK_CONTROL_H_
