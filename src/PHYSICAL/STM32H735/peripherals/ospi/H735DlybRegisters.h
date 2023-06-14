#ifndef _H735_OCTO_DLYB_REGISTERS_H_
#define _H735_OCTO_DLYB_REGISTERS_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   H735DlybRegisters
// @dscr:     Documented by PM0468 Pragramming manual, section 27.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct H735DlybRegisters
{
  volatile uint32_t DLYB_CR;                                    // 0x00
  volatile uint32_t DLYB_CFGR;                                  // 0x04
};

#define OCTO_DLYB_1_REGISTERS                                   (*(H735DlybRegisters *) 0x52006000UL)
#define OCTO_DLYB_2_REGISTERS                                   (*(H735DlybRegisters *) 0x5200B000UL)

#endif  // _H735_OCTO_DLYB_REGISTERS_H_

