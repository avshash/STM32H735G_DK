#ifndef _DISC735_OCTO_SPI_2_H_
#define _DISC735_OCTO_SPI_2_H_

#include "PhysicalGpioAf.h"
#include "PhysicalOctoSpi.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735OctoSpi2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735OctoSpi2 : public PhysicalOctoSpi
{
public:
  Disc735OctoSpi2 ();

  virtual void activateGpio () final;

private:
  PhysicalGpioAf m_cs;
  PhysicalGpioAf m_d0;
  PhysicalGpioAf m_d1;
  PhysicalGpioAf m_d2;
  PhysicalGpioAf m_d3;
  PhysicalGpioAf m_d4;
  PhysicalGpioAf m_d5;
  PhysicalGpioAf m_d6;
  PhysicalGpioAf m_d7;
  PhysicalGpioAf m_dqs;
  PhysicalGpioAf m_clk;
};

typedef Disc735OctoSpi2 PlatformOctoSpi2;

#endif  // _DISC735_OCTO_SPI_2_H_

