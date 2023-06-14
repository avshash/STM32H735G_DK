#ifndef _OSPI_COMMAND_PARAMS_H_
#define _OSPI_COMMAND_PARAMS_H_

#include "TypeOspiMode.h"
#include "TypeOspiInstruction.h"
#include "TypeOspiAddress.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   OspiCommandParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct OspiCommandParams
{
  TypeOspiMode m_mode;
  TypeOspiInstruction m_instruction_type;
  uint32_t m_instruction;
  TypeOspiAddress m_address_type;
  uint32_t m_address;
  uint32_t m_data[8];
  uint8_t m_data_bytes_len;
  uint8_t m_dummy_cycles;
};

#endif  // _OSPI_COMMAND_PARAMS_H_
