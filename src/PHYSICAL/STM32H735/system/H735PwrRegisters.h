#ifndef _H735_PWR_REGISTERS_H_
#define _H735_PWR_REGISTERS_H_

#define PWR_REGISTERS                                           (*(PhysicalPwrRegisters *) 0x58024800UL)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735PwrRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 6.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735PwrRegisters
{
  volatile uint32_t CR1;                                        // 0x00
  volatile uint32_t CSR1;                                       // 0x04
  volatile uint32_t CR2;                                        // 0x08
  volatile uint32_t CR3;                                        // 0x0C
  volatile uint32_t CPUCR;                                      // 0x10
  uint32_t m_place_holder_0x014;
  volatile uint32_t D3CR;                                       // 0x18
  uint32_t m_place_holder_0x01C;
  volatile uint32_t WKUPCR;                                     // 0x20
  volatile uint32_t WKUPFR;                                     // 0x24
  volatile uint32_t WKUPEPR;                                    // 0x28
};

typedef H735PwrRegisters PhysicalPwrRegisters;

#endif  // _H735_PWR_REGISTERS_H_
