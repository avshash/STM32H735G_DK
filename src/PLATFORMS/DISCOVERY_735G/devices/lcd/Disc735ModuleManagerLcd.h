#ifndef _DISC735_MODULE_MANAGER_LCD_H_
#define _DISC735_MODULE_MANAGER_LCD_H_

#include "ModuleManagerLcd.h"
#include "PlatformLtdc.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerLcd
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735ModuleManagerLcd : public ModuleManagerLcd
{
public:
  Disc735ModuleManagerLcd ();

private:
  virtual uint32_t getParamValue (TypeLcdParam) const final;
  virtual uint32_t getParamValue (TypeLtdcParam) const final;

  PlatformLtdc m_ltdc;
};

typedef Disc735ModuleManagerLcd PlatformModuleManagerLcd;

#endif  // _DISC735_MODULE_MANAGER_LCD_H_

