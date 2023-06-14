#ifndef _H735_MDMA_REGISTERS_H_
#define _H735_MDMA_REGISTERS_H_

#define MDMA_REGISTER_GISR0                                     (*(volatile uint32_t *) 0x52000000UL)
#define MDMA_REGISTER(channel)                                  (((PhysicalMdmaRegisters *) 0x52000000)[(channel) + 1])

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735MdmaRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 14.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735MdmaRegisters
{
  volatile uint32_t CxISR;                                     // 0x00
  volatile uint32_t CxIFCR;                                    // 0x04
  volatile uint32_t CxESR;                                     // 0x08
  volatile uint32_t CxCR;                                      // 0x0C
  volatile uint32_t CxTCR;                                     // 0x10
  volatile uint32_t CxBNDTR;                                   // 0x14
  volatile uint32_t CxSAR;                                     // 0x18
  volatile uint32_t CxDAR;                                     // 0x1C
  volatile uint32_t CxBRUR;                                    // 0x20
  volatile uint32_t CxLAR;                                     // 0x24
  volatile uint32_t CxTBR;                                     // 0x28
  uint32_t m_place_holder_0x02C;
  volatile uint32_t CxMAR;                                     // 0x30
  volatile uint32_t CxMDR;                                     // 0x34
  uint32_t m_place_holder_0x038[2];
};

typedef H735MdmaRegisters PhysicalMdmaRegisters;

#endif  // _H735_MDMA_REGISTERS_H_
