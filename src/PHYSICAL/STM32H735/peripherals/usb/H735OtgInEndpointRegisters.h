#ifndef _H735_OTG_IN_ENDPOINT_REGISTERS_H_
#define _H735_OTG_IN_ENDPOINT_REGISTERS_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735OtgInEndpointRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 62.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735OtgInEndpointRegisters
{
  volatile uint32_t DIEPCTL;                                   // 0x000
  uint32_t m_place_holder_0x004;
  volatile uint32_t DIEPINT;                                   // 0x008
  uint32_t m_place_holder_0x00C;
  volatile uint32_t DIEPTSIZ;                                  // 0x010
  volatile uint32_t DIEPDMA;                                   // 0x014
  volatile uint32_t DTXFSTS;                                   // 0x018
  uint32_t m_place_holder_0x01C;
};

typedef H735OtgInEndpointRegisters PhysicalOtgInEndpointRegisters;

#endif  // _H735_OTG_IN_ENDPOINT_REGISTERS_H_
