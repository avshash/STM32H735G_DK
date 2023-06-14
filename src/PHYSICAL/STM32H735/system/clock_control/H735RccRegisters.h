#ifndef _H735_RCC_REGISTERS_H_
#define _H735_RCC_REGISTERS_H_

#define RCC_REGISTERS                                           (*(PhysicalRccRegisters *) 0x58024400UL)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735RccRegisters
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735RccRegisters
{
  volatile uint32_t CR;
  volatile uint32_t HSICFGR;
  volatile uint32_t CRRCR;
  volatile uint32_t CSICFGR;
  volatile uint32_t CFGR;
  uint32_t m_place_holder_0x014;
  volatile uint32_t D1CFGR;
  volatile uint32_t D2CFGR;
  volatile uint32_t D3CFGR;
  uint32_t m_place_holder_0x024;
  volatile uint32_t PLLCKSELR;
  volatile uint32_t PLLCFGR;
  volatile uint32_t PLL1DIVR;
  volatile uint32_t PLL1FRACR;
  volatile uint32_t PLL2DIVR;
  volatile uint32_t PLL2FRACR;
  volatile uint32_t PLL3DIVR;
  volatile uint32_t PLL3FRACR;
  uint32_t m_place_holder_0x048;
  volatile uint32_t D1CCIPR;
  volatile uint32_t D2CCIP1R;
  volatile uint32_t D2CCIP2R;
  volatile uint32_t D3CCIPR;
  uint32_t m_place_holder_0x05C;
  volatile uint32_t CIER;
  volatile uint32_t CIFR;
  volatile uint32_t CICR;
  uint32_t m_place_holder_0x06C;
  volatile uint32_t BDCR;
  volatile uint32_t CSR;
  uint32_t m_place_holder_0x078;
  volatile uint32_t AHB3RSTR;
  volatile uint32_t AHB1RSTR;
  volatile uint32_t AHB2RSTR;
  volatile uint32_t AHB4RSTR;
  volatile uint32_t APB3RSTR;
  volatile uint32_t APB1LRSTR;
  volatile uint32_t APB1HRSTR;
  volatile uint32_t APB2RSTR;
  volatile uint32_t APB4RSTR;
  volatile uint32_t GCR;
  uint32_t m_place_holder_0x0A4;
  volatile uint32_t D3AMR;
  uint32_t m_place_holder_0x0AC[9];
  volatile uint32_t RSR;
  volatile uint32_t AHB3ENR;
  volatile uint32_t AHB1ENR;
  volatile uint32_t AHB2ENR;
  volatile uint32_t AHB4ENR;
  volatile uint32_t APB3ENR;
  volatile uint32_t APB1LENR;
  volatile uint32_t APB1HENR;
  volatile uint32_t APB2ENR;
  volatile uint32_t APB4ENR;
  uint32_t m_place_holder_0x0F8;
  volatile uint32_t AHB3LPENR;
  volatile uint32_t AHB1LPENR;
  volatile uint32_t AHB2LPENR;
  volatile uint32_t AHB4LPENR;
  volatile uint32_t APB3LPENR;
  volatile uint32_t APB1LLPENR;
  volatile uint32_t APB1HLPENR;
  volatile uint32_t APB2LPENR;
  volatile uint32_t APB4LPENR;
  uint32_t m_place_holder_0x120[4];
  volatile uint32_t C1_RSR;
  volatile uint32_t C1_AHB3ENR;
  volatile uint32_t C1_AHB1ENR;
  volatile uint32_t C1_AHB2ENR;
  volatile uint32_t C1_AHB4ENR;
  volatile uint32_t C1_APB3ENR;
  volatile uint32_t C1_APB1LENR;
  volatile uint32_t C1_APB1HENR;
  volatile uint32_t C1_APB2ENR;
  volatile uint32_t C1_APB4ENR;
  uint32_t m_place_holder_0x158;
  volatile uint32_t C1_AHB3LPENR;
  volatile uint32_t C1_AHB1LPENR;
  volatile uint32_t C1_AHB2LPENR;
  volatile uint32_t C1_AHB4LPENR;
  volatile uint32_t C1_APB3LPENR;
  volatile uint32_t C1_APB1LLPENR;
  volatile uint32_t C1_APB1HLPENR;
  volatile uint32_t C1_APB2LPENR;
  volatile uint32_t C1_APB4LPENR;
  uint32_t m_padding[32];
};

typedef H735RccRegisters PhysicalRccRegisters;

#endif  // _H735_RCC_REGISTERS_H_

