#define FLASH_REGISTERS                                         (*(PhysicalFlashRegisters *) 0x52002000UL)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735FlashRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 4.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735FlashRegisters
{
  volatile uint32_t ACR;                                        // 0x000
  volatile uint32_t KEYR;                                       // 0x004
  volatile uint32_t OPTKEYR;                                    // 0x008
  volatile uint32_t CR;                                         // 0x00C
  volatile uint32_t SR;                                         // 0x010
  volatile uint32_t CCR;                                        // 0x014
  volatile uint32_t OPTCR;                                      // 0x018
  volatile uint32_t OPTSR_CUR;                                  // 0x01C
  volatile uint32_t OPTSR_PRG;                                  // 0x020
  volatile uint32_t OPTCCR;                                     // 0x024
  volatile uint32_t PRAR_CUR;                                   // 0x028
  volatile uint32_t PRAR_PRG;                                   // 0x03C
  volatile uint32_t SCAR_CUR;                                   // 0x030
  volatile uint32_t SCAR_PRG;                                   // 0x034
  volatile uint32_t WPSN_CUR;                                   // 0x038
  volatile uint32_t WPSN_PRG;                                   // 0x03C
  volatile uint32_t BOOT_CUR;                                   // 0x040
  volatile uint32_t BOOT_PRG;                                   // 0x044
  uint32_t m_place_holder_0x048;
  uint32_t m_place_holder_0x04C;
  volatile uint32_t CRCCR;                                      // 0x050
  volatile uint32_t CRCSADDR;                                   // 0x054
  volatile uint32_t CRCEADDR;                                   // 0x058
  volatile uint32_t CRCDATAR;                                   // 0x05C
  volatile uint32_t ECC_FAR;                                    // 0x060
  uint32_t m_place_holder_0x064;
  uint32_t m_place_holder_0x068;
  uint32_t m_place_holder_0x06C;
  volatile uint32_t OPTSR2_CUR;                                 // 0x070
  volatile uint32_t OPTSR2_PRG;                                 // 0x074
};

