#include "Common.h"
#include "spi8_action.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8Action
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Spi8Action::Spi8Action (TypeSpi8Bank bank_type, TypeSpi8Action action_type, TypeSpi8Lines lines_type)
: m_physical (bank_type, action_type, lines_type)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8Action
// @method:   setInstruction
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Spi8Action::setInstruction (TypeSpi8ParamSize instruction_size, uint32_t instruction)
{
  m_physical.setInstruction (instruction_size, instruction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8Action
// @method:   setDummyCycles
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Spi8Action::setDummyCycles (uint32_t dummy_cycles)
{
  m_physical.setDummyCycles (dummy_cycles);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8Action
// @method:   setAddress
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Spi8Action::setAddress (TypeSpi8ParamSize address_size, uint32_t address)
{
  m_physical.setAddress (address_size, address);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8Action
// @method:   commitActionSend
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
Spi8Action::commitActionSend (const uint32_t * data, uint16_t data_size, bool b_dtr_enabled)
{
  return m_physical.commitActionSend (data, data_size, b_dtr_enabled);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8Action
// @method:   commitActionReceive
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
Spi8Action::commitActionReceive (uint32_t * data, uint16_t data_size, bool b_dtr_enabled)
{
  return m_physical.commitActionReceive (data, data_size, b_dtr_enabled);
}

