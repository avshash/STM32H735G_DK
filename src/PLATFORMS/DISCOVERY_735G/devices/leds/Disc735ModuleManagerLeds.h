#ifndef _DISC735_MODULE_MANAGER_LEDS_H_
#define _DISC735_MODULE_MANAGER_LEDS_H_

#include "ModuleManagerLeds.h"
#include "PhysicalGpioOut.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerLeds
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735ModuleManagerLeds : public ModuleManagerLeds
{
public:
  Disc735ModuleManagerLeds ();

  virtual void setLed (TypeLedColor, bool b_on) final;

protected:
  virtual void activateGpio () final;

private:
  PhysicalGpioOut m_red;
  PhysicalGpioOut m_green;
};

typedef Disc735ModuleManagerLeds PlatformModuleManagerLeds;

#endif  // _DISC735_MODULE_MANAGER_LEDS_H_

