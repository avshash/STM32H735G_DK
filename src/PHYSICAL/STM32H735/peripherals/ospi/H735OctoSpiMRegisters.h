#ifndef _H735_OCTO_SPI_M_REGISTERS_H_
#define _H735_OCTO_SPI_M_REGISTERS_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735OctoSpiMRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 26.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735OctoSpiMRegisters
{
  volatile uint32_t CR;                                         // 0x00
  volatile uint32_t P1CR;                                       // 0x04
  volatile uint32_t P2CR;                                       // 0x08
};

#define OCTO_SPIM_REGISTERS                                     (*(H735OctoSpiMRegisters *) 0x5200B400UL)

#endif  // _H735_OCTO_SPI_M_REGISTERS_H_

