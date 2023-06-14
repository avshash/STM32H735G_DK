#ifndef _H735_NVIC_REGISTERS_H_
#define _H735_NVIC_REGISTERS_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735NvicRegisters
// @dscr:     Documented by PM0253 Pragramming manual.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735NvicRegisters
{
  volatile uint32_t NVIC_ISER[8];                             // 0x100
  uint32_t m_place_holder_0x120[0x18];
  volatile uint32_t NVIC_ICER[8];                             // 0x180
  uint32_t m_place_holder_0x1A0[0x18];
  volatile uint32_t NVIC_ISPR[8];                             // 0x200
  uint32_t m_place_holder_0x220[0x18];
  volatile uint32_t NVIC_ICPR[8];                             // 0x280
  uint32_t m_place_holder_0x2A0[0x18];
  volatile uint32_t NVIC_IABR[8];                             // 0x300
  uint32_t m_place_holder_0x320[0x38];
  volatile uint32_t NVIC_IPR[60];                             // 0x400
  uint32_t m_place_holder_0x4F0[0x204];
};

typedef H735NvicRegisters PhysicalNvicRegisters;

#endif  //_H735_NVIC_REGISTERS_H_
