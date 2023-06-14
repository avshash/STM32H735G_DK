#ifndef _H735_OCTO_SPI_REGISTERS_H_
#define _H735_OCTO_SPI_REGISTERS_H_

#define OCTO_SPI_1_REGISTERS                                    (*(H735OctoSpiRegisters *) 0x52005000UL)
#define OCTO_SPI_2_REGISTERS                                    (*(H735OctoSpiRegisters *) 0x5200A000UL)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735OctoSpiRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 25.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735OctoSpiRegisters
{
  volatile uint32_t CR;                                         // 0x000
  uint32_t m_place_holder_0x004;
  volatile uint32_t DCR1;                                       // 0x008
  volatile uint32_t DCR2;                                       // 0x00C
  volatile uint32_t DCR3;                                       // 0x010
  volatile uint32_t DCR4;                                       // 0x014
  uint32_t m_place_holder_0x018[2];
  volatile uint32_t SR;                                         // 0x020
  volatile uint32_t FCR;                                        // 0x024
  uint32_t m_place_holder_0x028[6];
  volatile uint32_t DLR;                                        // 0x040
  uint32_t m_place_holder_0x044;
  volatile uint32_t AR;                                         // 0x048
  uint32_t m_place_holder_0x04C;
  volatile uint32_t DR;                                         // 0x050
  uint32_t m_place_holder_0x054[11];
  volatile uint32_t PSMKR;                                      // 0x080
  uint32_t m_place_holder_0x084;
  volatile uint32_t PSMAR;                                      // 0x088
  uint32_t m_place_holder_0x08C;
  volatile uint32_t PIR;                                        // 0x090
  uint32_t m_place_holder_0x094[27];
  volatile uint32_t CCR;                                        // 0x0100
  uint32_t m_place_holder_0x104;
  volatile uint32_t TCR;                                        // 0x0108
  uint32_t m_place_holder_0x10C;
  volatile uint32_t IR;                                         // 0x0110
  uint32_t m_place_holder_0x114[3];
  volatile uint32_t ABR;                                        // 0x0120
  uint32_t m_place_holder_0x124[3];
  volatile uint32_t LPTR;                                       // 0x0130
  uint32_t m_place_holder_0x134[3];
  volatile uint32_t WPCCR;                                      // 0x0140
  uint32_t m_place_holder_0x144;
  volatile uint32_t WPTCR;                                      // 0x0148
  uint32_t m_place_holder_0x14C;
  volatile uint32_t WPIR;                                       // 0x0150
  uint32_t m_place_holder_0x154[3];
  volatile uint32_t WPABR;                                      // 0x0160
  uint32_t m_place_holder_0x164[7];
  volatile uint32_t WCCR;                                       // 0x0180
  uint32_t m_place_holder_0x184;
  volatile uint32_t WTCR;                                       // 0x0188
  uint32_t m_place_holder_0x18C;
  volatile uint32_t WOR;                                        // 0x0190
  uint32_t m_place_holder_0x194[3];
  volatile uint32_t WABR;                                       // 0x01A0
  uint32_t m_place_holder_0x1A4[23];
  volatile uint32_t HLCR;                                       // 0x0200
};

#endif  // _H735_OCTO_SPI_REGISTERS_H_

