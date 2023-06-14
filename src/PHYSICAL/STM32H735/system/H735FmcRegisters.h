#define FMC_REGISTERS                                           (*(PhysicalFmcRegisters *) 0x52004000UL)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735FmcRegisters
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735FmcRegisters
{
  volatile uint32_t BCR1;                                       // 0x000
  volatile uint32_t BTR1;                                       // 0x004
  volatile uint32_t BCR2;                                       // 0x008
  volatile uint32_t BTR2;                                       // 0x00C
  volatile uint32_t BCR3;                                       // 0x010
  volatile uint32_t BTR3;                                       // 0x014
  volatile uint32_t BCR4;                                       // 0x018
  volatile uint32_t BTR4;                                       // 0x01C
};


