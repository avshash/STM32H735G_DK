#ifndef _LTDC_H_
#define _LTDC_H_

#include "TypeLtdcData.h"

class ModuleManagerLcd;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Ltdc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Ltdc
{
public:
  virtual void setupRcc (const ModuleManagerLcd &) = 0;
  virtual void activateGpio () = 0;
  virtual void configure (const ModuleManagerLcd &) = 0;
  virtual void setActiveDisplay (TypeLtdcData, const uint16_t * base_address, uint32_t width, uint32_t height) = 0;
  virtual void verifyDisplayGood () = 0;
};

#endif  // _LTDC_H_
