#ifndef _MACRONIX_INSTRUCTION_H_
#define _MACRONIX_INSTRUCTION_H_

#include "TypeMacronixCommand.h"
#include "OspiCommandParams.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   MacronixInstruction
// @note:     Define as structure instead of class for global initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct MacronixInstruction
{
public:
  TypeMacronixCommand m_command_id;
  TypeOspiMode m_command_mode;
  TypeOspiInstruction m_instruction_type;
  uint16_t m_instruction_value;
  uint8_t m_dummy_cycles;

  static void initParams (OspiCommandParams &, TypeMacronixCommand, uint8_t default_dummy_cycles);

private:
  static const MacronixInstruction g_instruction_map[];
};

#endif  // _MACRONIX_INSTRUCTION_H_
