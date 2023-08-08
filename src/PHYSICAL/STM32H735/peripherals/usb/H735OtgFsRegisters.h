#ifndef _H735_OTG_FS_REGISTERS_H_
#define _H735_OTG_FS_REGISTERS_H_

#include "H735OtgFsChannelRegisters.h"
#include "H735OtgInEndpointRegisters.h"
#include "H735OtgOutEndpointRegisters.h"

#define OTG_FS_REGISTERS                                        (*(PhysicalOtgFsRegisters *) 0x40040000UL)
#define OTG_FS_CHANNEL_REGISTERS(index)                         (OTG_FS_REGISTERS.m_host_channels[index])
#define OTG_FS_IN_ENDPOINTS_REGISTERS(index)                    (OTG_FS_REGISTERS.m_in_endpoints[index])
#define OTG_FS_OUT_ENDPOINTS_REGISTERS(index)                   (OTG_FS_REGISTERS.m_out_endpoints[index])
#define OTG_FS_CHANNEL_BUFFER(index)                            ((uint32_t *) (0x40041000UL + (0x1000 *index)))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735OtgFsRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 62.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735OtgFsRegisters
{
  volatile uint32_t GOTGCTL;                                   // 0x0000
  volatile uint32_t GOTGINT;                                   // 0x0004
  volatile uint32_t GAHBCFG;                                   // 0x0008
  volatile uint32_t GUSBCFG;                                   // 0x000C
  volatile uint32_t GRSTCTL;                                   // 0x0010
  volatile uint32_t GINTSTS;                                   // 0x0014
  volatile uint32_t GINTMSK;                                   // 0x0018
  volatile uint32_t GRXSTSR;                                   // 0x001C
  volatile uint32_t GRXSTSP;                                   // 0x0020
  volatile uint32_t GRXFSIZ;                                   // 0x0024
  volatile uint32_t HNPTXFSIZ;                                 // 0x0028
  volatile uint32_t HNPTXSTS;                                  // 0x002C
  uint32_t m_place_holder_0x0030[2];
  volatile uint32_t GCCFG;                                     // 0x0038
  volatile uint32_t CID;                                       // 0x003C
  uint32_t m_place_holder_0x0040[5];
  volatile uint32_t GLPMCFG;                                   // 0x0054
  uint32_t m_place_holder_0x0058[42];
  volatile uint32_t HPTXFSIZ;                                  // 0x0100
  volatile uint32_t DIEPTXF1;                                  // 0x0104
  volatile uint32_t DIEPTXF2;                                  // 0x0108
  volatile uint32_t DIEPTXF3;                                  // 0x010C
  volatile uint32_t DIEPTXF4;                                  // 0x0110
  volatile uint32_t DIEPTXF5;                                  // 0x0114
  volatile uint32_t DIEPTXF6;                                  // 0x0118
  volatile uint32_t DIEPTXF7;                                  // 0x011C
  volatile uint32_t DIEPTXF8;                                  // 0x0120
  uint32_t m_place_holder_0x0124[183];
  volatile uint32_t HCFG;                                      // 0x0400
  volatile uint32_t HFIR;                                      // 0x0404
  volatile uint32_t HFNUM;                                     // 0x0408
  uint32_t m_place_holder_0x040C;
  volatile uint32_t HPTXSTS;                                   // 0x0410
  volatile uint32_t HAINT;                                     // 0x0414
  volatile uint32_t HAINTMSK;                                  // 0x0418
  volatile uint32_t HFLBADDR;                                  // 0x041C
  uint32_t m_place_holder_0x0420[8];
  volatile uint32_t HPRT;                                      // 0x0440
  uint32_t m_place_holder_0x0444[47];
  PhysicalOtgFsChannelRegisters m_host_channels[16];           // 0x0500
  uint32_t m_place_holder_0x0700[64];
  volatile uint32_t DCFG;                                      // 0x0800
  volatile uint32_t DCTL;                                      // 0x0804
  volatile uint32_t DSTS;                                      // 0x0808
  uint32_t m_place_holder_0x080C;
  volatile uint32_t DIEPMSK;                                   // 0x0810
  volatile uint32_t DOEPMSK;                                   // 0x0814
  volatile uint32_t DAINT;                                     // 0x0818
  volatile uint32_t DAINTMSK;                                  // 0x081C
  uint32_t m_place_holder_0x0820[2];
  volatile uint32_t DVBUSDIS;                                  // 0x0828
  volatile uint32_t DVBUSPULE;                                 // 0x082C
  volatile uint32_t DTHRCTL;                                   // 0x0830
  volatile uint32_t PEMPMSK;                                   // 0x0834
  volatile uint32_t DEACHINT;                                  // 0x0838
  volatile uint32_t DEACHINTMSK;                               // 0x083C
  uint32_t m_place_holder_0x0840;
  volatile uint32_t HS_DIEPEACHMSK1;                           // 0x0844
  uint32_t m_place_holder_0x0848[15];
  volatile uint32_t HS_DOEPEACHMSK1;                           // 0x0884
  uint32_t m_place_holder_0x0888[30];
  PhysicalOtgInEndpointRegisters m_in_endpoints[9];            // 0x0900
  uint32_t m_place_holder_0x0A20[56];
  PhysicalOtgOutEndpointRegisters m_out_endpoints[9];          // 0x0B00
  uint32_t m_place_holder_0x0C20[120];
  volatile uint32_t PCGCCTL;                                   // 0x0E00
};

typedef H735OtgFsRegisters PhysicalOtgFsRegisters;

#endif  // _H735_OTG_FS_REGISTERS_H_
