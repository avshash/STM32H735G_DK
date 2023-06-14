#ifndef _H735_I2C_REGISTERS_H_
#define _H735_I2C_REGISTERS_H_

#define I2C_1_REGISTERS                                         (*(H735I2cRegisters *) 0x40005400UL)
#define I2C_2_REGISTERS                                         (*(H735I2cRegisters *) 0x40005800UL)
#define I2C_3_REGISTERS                                         (*(H735I2cRegisters *) 0x40005C00UL)
#define I2C_4_REGISTERS                                         (*(H735I2cRegisters *) 0x58001C00UL)
#define I2C_5_REGISTERS                                         (*(H735I2cRegisters *) 0x40006400UL)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735I2cRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 52.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735I2cRegisters
{
  volatile uint32_t CR1;                                        // 0x000
  volatile uint32_t CR2;                                        // 0x004
  volatile uint32_t OAR1;                                       // 0x008
  volatile uint32_t OAR2;                                       // 0x00C
  volatile uint32_t TIMINGR;                                    // 0x010
  volatile uint32_t TIMEOUTR;                                   // 0x014
  volatile uint32_t ISR;                                        // 0x018
  volatile uint32_t ICR;                                        // 0x01C
  volatile uint32_t PECR;                                       // 0x020
  volatile uint32_t RXDR;                                       // 0x024
  volatile uint32_t TXDR;                                       // 0x028
};

#endif  // _H735_I2C_REGISTERS_H_

