#ifndef _H735_OTG_OUT_ENDPOINT_REGISTERS_H_
#define _H735_OTG_OUT_ENDPOINT_REGISTERS_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735OtgOutEndpointRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 62.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735OtgOutEndpointRegisters
{
  volatile uint32_t DOEPCTL;                                   // 0x000
  uint32_t m_place_holder_0x004;
  volatile uint32_t DOEPINT;                                   // 0x008
  uint32_t m_place_holder_0x00C;
  volatile uint32_t DOEPTSIZ;                                  // 0x010
  volatile uint32_t DOEPDMA;                                   // 0x014
  uint32_t m_place_holder_0x018[2];
};

typedef H735OtgOutEndpointRegisters PhysicalOtgOutEndpointRegisters;

#endif  // _H735_OTG_OUT_ENDPOINT_REGISTERS_H_
