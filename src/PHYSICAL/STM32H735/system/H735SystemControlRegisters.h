#ifndef _H735_SYSTEM_CONTROL_REGISTERS_H_
#define _H735_SYSTEM_CONTROL_REGISTERS_H_

#define SYSTEM_CONTROL_REGISTERS                      (*(PhysicalSystemControlRegisters *) 0xE000E000UL)
#define NVIC_REGISTERS                                (SYSTEM_CONTROL_REGISTERS.m_nvic_registers)
#define MPU_REGISTERS                                 (SYSTEM_CONTROL_REGISTERS.m_mpu_registers)

#include "PhysicalNvicRegisters.h"
#include "PhysicalMpuRegisters.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735SystemControlRegisters
// @dscr:     Documented by PM0253 Pragramming manual.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735SystemControlRegisters
{
  uint32_t m_place_holder_0x000;
  uint32_t m_place_holder_0x004;
  volatile uint32_t ACTLR;                                    // 0x008
  uint32_t m_place_holder_0x00C;

  volatile uint32_t SYST_CSR;                                 // 0x010
  volatile uint32_t SYST_RVR;                                 // 0x014
  volatile uint32_t SYST_CVR;                                 // 0x018
  volatile uint32_t SYST_CALIB;                               // 0x01C
  uint32_t m_place_holder_0x020[0x38];

  H735NvicRegisters m_nvic_registers;                         // 0x100

  volatile uint32_t CPUID;                                    // 0xD00
  volatile uint32_t ICSR;                                     // 0xD04
  volatile uint32_t VTOR;                                     // 0xD08
  volatile uint32_t AIRCR;                                    // 0xD0C
  volatile uint32_t SCR;                                      // 0xD10
  volatile uint32_t CCR;                                      // 0xD14
  volatile uint32_t SHPR1;                                    // 0xD18
  volatile uint32_t SHPR2;                                    // 0xD1C
  volatile uint32_t SHPR3;                                    // 0xD20
  volatile uint32_t SHCSR;                                    // 0xD24
  volatile uint32_t CFSR;                                     // 0xD28
  volatile uint32_t HFSR;                                     // 0xD2C
  uint32_t m_place_holder_0xD30;
  volatile uint32_t MMAR;                                     // 0xD34
  volatile uint32_t BFAR;                                     // 0xD38
  volatile uint32_t AFSR;                                     // 0xD3C
  uint32_t m_place_holder_0xD40[0x0E];

  volatile uint32_t CLIDR;                                    // 0xD78
  volatile uint32_t CTR;                                      // 0xD7C
  volatile uint32_t CCSIDR;                                   // 0xD80
  volatile uint32_t CSSELR;                                   // 0xD84
  volatile uint32_t CPACR;                                    // 0xD88
  uint32_t m_place_holder_0xD8C;

  H735MpuRegisters m_mpu_registers;                           // 0xD90

  volatile uint32_t STIR;                                     // 0xF00
  uint32_t m_place_holder_0xF04[0x0C];

  volatile uint32_t FPCCR;                                    // 0xF34
  volatile uint32_t FPCAR;                                    // 0xF38
  volatile uint32_t FPDSCR;                                   // 0xF3C
  uint32_t m_place_holder_0xF40[4];

  volatile uint32_t ICIALLU;                                  // 0xF50
  uint32_t m_place_holder_0xF54;
  volatile uint32_t ICIMVAU;                                  // 0xF58
  volatile uint32_t DCIMVAC;                                  // 0xF5C
  volatile uint32_t DCISW;                                    // 0xF60
  volatile uint32_t DCCMVAU;                                  // 0xF64
  volatile uint32_t DCCMVAC;                                  // 0xF68
  volatile uint32_t DCCSW;                                    // 0xF6C
  volatile uint32_t DCCIMVAC;                                 // 0xF70
  volatile uint32_t DCCISW;                                   // 0xF74
  volatile uint32_t BPIALL;                                   // 0xF78
  uint32_t m_place_holder_0xF7C[5];

  volatile uint32_t ITCMCR;                                   // 0xF90
  volatile uint32_t DTCMCR;                                   // 0xF94
  volatile uint32_t AHBPCR;                                   // 0xF98
  uint32_t m_place_holder_0xF9C;
  volatile uint32_t CACR;                                     // 0xFA0
  uint32_t m_place_holder_0xFA4;
  volatile uint32_t AHBSCR;                                   // 0xFA8
};

typedef H735SystemControlRegisters PhysicalSystemControlRegisters;

#endif  // _H735_SYSTEM_CONTROL_REGISTERS_H_
