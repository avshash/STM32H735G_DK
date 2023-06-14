#define GPIO_REGISTERS_VECTOR                                   ((PhysicalGpioRegisters *) 0x58020000UL)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735GpioRegisters
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735GpioRegisters
{
  uint32_t MODER;
  uint32_t OTYPER;
  uint32_t OSPEEDR;
  uint32_t PUPDR;
  uint32_t IDR;
  uint32_t ODR;
  uint32_t BSRR;
  uint32_t LCKR;
  uint32_t AFR[2];
  uint32_t padding[0xF6];
};

