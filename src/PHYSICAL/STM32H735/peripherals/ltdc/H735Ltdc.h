#ifndef _H735_LTDC_H_
#define _H735_LTDC_H_

#include "Ltdc.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Ltdc
// @dscr:     Behavior is documented by PM0468 Pragramming manual, section 38.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735Ltdc : public Ltdc
{
protected:
  H735Ltdc ();

  virtual void configure (const ModuleManagerLcd &) final;

private:
  virtual void setupRcc (const ModuleManagerLcd &) final;
  virtual void setActiveDisplay (TypeLtdcData, const uint16_t * base_address, uint32_t width, uint32_t height) final;
  virtual void verifyDisplayGood () final;

  void configureSync (const ModuleManagerLcd &);
  void configureLayer1 (TypeLtdcData, uint32_t width, uint32_t height);
  unsigned int configureFormat (TypeLtdcData ltdc_type);
};

typedef H735Ltdc PhysicalLtdc;

#endif  // _H735_LTDC_H_

