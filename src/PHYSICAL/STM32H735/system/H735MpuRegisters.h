#ifndef _H735_MPU_REGISTERS_H_
#define _H735_MPU_REGISTERS_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735MpuRegisters
// @dscr:     Documented by PM0253 Pragramming manual.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735MpuRegisters
{
  volatile uint32_t TYPE;                                     // 0xD90
  volatile uint32_t CTRL;                                     // 0xD94
  volatile uint32_t RNR;                                      // 0xD98
  volatile uint32_t RBAR;                                     // 0xD9C
  volatile uint32_t RASR;                                     // 0xDA0
  volatile uint32_t RBAR_A1;                                  // 0xDA4
  volatile uint32_t RASR_A1;                                  // 0xDA8
  volatile uint32_t RBAR_A2;                                  // 0xDAC
  volatile uint32_t RASR_A2;                                  // 0xDB0
  volatile uint32_t RBAR_A3;                                  // 0xDB4
  volatile uint32_t RASR_A3;                                  // 0xDB8
  uint32_t m_place_holder_0xDBC[0x51];
};

typedef H735MpuRegisters PhysicalMpuRegisters;

#endif  // _H735_MPU_REGISTERS_H_
