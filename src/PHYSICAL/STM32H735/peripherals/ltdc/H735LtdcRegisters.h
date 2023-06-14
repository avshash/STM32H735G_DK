#ifndef _H735_LTDC_REGISTERS_H_
#define _H735_LTDC_REGISTERS_H_

#define LTDC_REGISTER                                           (*(PhysicalLtdcRegisters *) 0x50001000)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735LtdcRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 38.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735LtdcRegisters
{
  uint32_t m_place_holder_0x000[2];
  volatile uint32_t SSCR;                                      // 0x008
  volatile uint32_t BPCR;                                      // 0x00C
  volatile uint32_t AWCR;                                      // 0x010
  volatile uint32_t TWCR;                                      // 0x014
  volatile uint32_t GCR;                                       // 0x018
  uint32_t m_place_holder_0x01C[2];
  volatile uint32_t SRCR;                                      // 0x024
  uint32_t m_place_holder_0x028;
  volatile uint32_t BCCR;                                      // 0x02C
  uint32_t m_place_holder_0x030;
  volatile uint32_t IER;                                       // 0x034
  volatile uint32_t ISR;                                       // 0x038
  volatile uint32_t ICR;                                       // 0x03C
  volatile uint32_t LIPCR;                                     // 0x040
  volatile uint32_t CPSR;                                      // 0x044
  volatile uint32_t CDSR;                                      // 0x048
  uint32_t m_place_holder_0x04C[14];
  volatile uint32_t L1CR;                                      // 0x084
  volatile uint32_t L1WHPCR;                                   // 0x088
  volatile uint32_t L1WVPCR;                                   // 0x08C
  volatile uint32_t L1CKCR;                                    // 0x090
  volatile uint32_t L1PFCR;                                    // 0x094
  volatile uint32_t L1CACR;                                    // 0x098
  volatile uint32_t L1DCCR;                                    // 0x09C
  volatile uint32_t L1BFCR;                                    // 0x0A0
  uint32_t m_place_holder_0x0A4[2];
  volatile uint32_t L1CFBAR;                                   // 0x0AC
  volatile uint32_t L1CFBLR;                                   // 0x0B0
  volatile uint32_t L1CFBLNR;                                  // 0x0B4
  uint32_t m_place_holder_0x0B8[3];
  volatile uint32_t L1CLUTWR;                                  // 0x0C4
  uint32_t m_place_holder_0x0C8[15];
  volatile uint32_t L2CR;                                      // 0x104
  volatile uint32_t L2WHPCR;                                   // 0x108
  volatile uint32_t L2WVPCR;                                   // 0x10C
  volatile uint32_t L2CKCR;                                    // 0x110
  volatile uint32_t L2PFCR;                                    // 0x114
  volatile uint32_t L2CACR;                                    // 0x118
  volatile uint32_t L2DCCR;                                    // 0x11C
  volatile uint32_t L2BFCR;                                    // 0x120
  uint32_t m_place_holder_0x124[2];
  volatile uint32_t L2CFBAR;                                   // 0x12C
  volatile uint32_t L2CFBLR;                                   // 0x130
  volatile uint32_t L2CFBLNR;                                  // 0x134
  uint32_t m_place_holder_0x138[3];
  volatile uint32_t L2CLUTWR;                                  // 0x0C4
};

typedef H735LtdcRegisters PhysicalLtdcRegisters;

#endif  // _H735_LTDC_REGISTERS_H_
