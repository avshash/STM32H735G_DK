#ifndef _H735_OTG_FS_CHANNEL_REGISTERS_H_
#define _H735_OTG_FS_CHANNEL_REGISTERS_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735OtgFsChannelRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 62.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735OtgFsChannelRegisters
{
  volatile uint32_t HCCHAR;                                    // 0x000
  volatile uint32_t HCSPLT;                                    // 0x004
  volatile uint32_t HCINT;                                     // 0x008
  volatile uint32_t HCINTMSK;                                  // 0x00C
  volatile uint32_t HCTSIZ;                                    // 0x010
  volatile uint32_t HCDMA;                                     // 0x014
  uint32_t m_place_holder_0x018;
  volatile uint32_t HCDMAB;                                    // 0x01C
};

typedef H735OtgFsChannelRegisters PhysicalOtgFsChannelRegisters;

#endif  // _H735_OTG_FS_CHANNEL_REGISTERS_H_
