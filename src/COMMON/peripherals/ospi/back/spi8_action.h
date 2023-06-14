#ifndef _SPI8_ACTION_H_
#define _SPI8_ACTION_H_

#include "physical_spi8_action.h"
#include "type_spi8_lines.h"
#include "type_spi8_param_size.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8Action
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Spi8Action
{
public:
  void setInstruction (TypeSpi8ParamSize instruction_size, uint32_t instruction);
  void setAddress (TypeSpi8ParamSize address_size, uint32_t address);
  void setDummyCycles (uint32_t dummy_cycles);

protected:
  Spi8Action (TypeSpi8Bank bank_type, TypeSpi8Action action_type, TypeSpi8Lines lines_type);

  bool commitActionSend (const uint32_t * data, uint16_t data_size, bool b_dtr_enabled);
  bool commitActionReceive (uint32_t * data, uint16_t data_size, bool b_dtr_enabled);

private:
  PhysicalSpi8Action m_physical;
};

#endif  // _SPI8_ACTION_H_
