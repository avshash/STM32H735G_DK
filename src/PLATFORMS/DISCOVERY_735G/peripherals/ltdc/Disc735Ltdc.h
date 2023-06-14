#ifndef _DISC735_LTDC_H_
#define _DISC735_LTDC_H_

#include "PhysicalGpioAf.h"
#include "PhysicalLtdc.h"
#include "PhysicalGpioAf.h"
#include "PhysicalGpioOut.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735Ltdc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735Ltdc : public PhysicalLtdc
{
public:
  Disc735Ltdc ();

private:
  virtual void activateGpio () final;

  PhysicalGpioAf m_clk;
  PhysicalGpioAf m_vsync;
  PhysicalGpioAf m_hsync;
  PhysicalGpioAf m_r0;
  PhysicalGpioAf m_r1;
  PhysicalGpioAf m_r2;
  PhysicalGpioAf m_r3;
  PhysicalGpioAf m_r4;
  PhysicalGpioAf m_r5;
  PhysicalGpioAf m_r6;
  PhysicalGpioAf m_r7;
  PhysicalGpioAf m_g0;
  PhysicalGpioAf m_g1;
  PhysicalGpioAf m_g2;
  PhysicalGpioAf m_g3;
  PhysicalGpioAf m_g4;
  PhysicalGpioAf m_g5;
  PhysicalGpioAf m_g6;
  PhysicalGpioAf m_g7;
  PhysicalGpioAf m_b0;
  PhysicalGpioAf m_b1;
  PhysicalGpioAf m_b2;
  PhysicalGpioAf m_b3;
  PhysicalGpioAf m_b4;
  PhysicalGpioAf m_b5;
  PhysicalGpioAf m_b6;
  PhysicalGpioAf m_b7;

  PhysicalGpioOut m_de;
  PhysicalGpioOut m_disp;
  PhysicalGpioOut m_bl_ctrl;
};

typedef Disc735Ltdc PlatformLtdc;

#endif  // _DISC735_LTDC_H_

