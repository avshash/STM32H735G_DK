#include "Common.h"
#include "MacronixInstruction.h"

#include <cstring>

// @note:     Follows documentation by Macronix 'P/N 2357' (for MX25LM51245G).
const MacronixInstruction MacronixInstruction::g_instruction_map[] =
{
  {MACRONIX_COMMAND_SPI_READ4B,     OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0013U, 0x00U},
  {MACRONIX_COMMAND_SPI_PP4B,       OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0012U, 0x00U},
  {MACRONIX_COMMAND_SPI_SE4B,       OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0021U, 0x00U},
  {MACRONIX_COMMAND_SPI_WREN,       OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0006U, 0x00U},
  {MACRONIX_COMMAND_SPI_RDID,       OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x009fU, 0x00U},
  {MACRONIX_COMMAND_SPI_RDSR,       OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0005U, 0x00U},
  {MACRONIX_COMMAND_SPI_RDCR,       OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0015U, 0x00U},
  {MACRONIX_COMMAND_SPI_WRSR,       OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0001U, 0x00U},
  {MACRONIX_COMMAND_SPI_RDCR2,      OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0071U, 0x00U},
  {MACRONIX_COMMAND_SPI_WRCR2,      OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0072U, 0x00U},
  {MACRONIX_COMMAND_SPI_RDSCUR,     OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x002BU, 0x00U},
  {MACRONIX_COMMAND_SPI_NOP,        OSPI_MODE_SPI, OSPI_INSTRUCTION_BYTES_1, 0x0000U, 0x00U},
  {MACRONIX_COMMAND_OPI_8READ,      OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0xEC13U, 0xFFU},
  {MACRONIX_COMMAND_OPI_RDID,       OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x9F60U, 0x04U},
  {MACRONIX_COMMAND_OPI_PP,         OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x12EDU, 0x00U},
  {MACRONIX_COMMAND_OPI_SE,         OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x21DEU, 0x00U},
  {MACRONIX_COMMAND_OPI_WREN,       OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x06F9U, 0x00U},
  {MACRONIX_COMMAND_OPI_RDSR,       OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x05FAU, 0x04U},
  {MACRONIX_COMMAND_OPI_RDCR,       OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x15EAU, 0x04U},
  {MACRONIX_COMMAND_OPI_WRSR_WRCR,  OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x01FEU, 0x00U},
  {MACRONIX_COMMAND_OPI_RDCR2,      OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x718EU, 0x04U},
  {MACRONIX_COMMAND_OPI_WRCR2,      OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x728DU, 0x00U},
  {MACRONIX_COMMAND_OPI_RDSCUR,     OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x2BD4U, 0x04U},
  {MACRONIX_COMMAND_OPI_NOP,        OSPI_MODE_STR, OSPI_INSTRUCTION_BYTES_2, 0x00FFU, 0x00U}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixInstruction
// @method:   initParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixInstruction::initParams (OspiCommandParams & command_params,
                                 TypeMacronixCommand command_type, uint8_t default_dummy_cycles)
{
  memset (&command_params, 0, sizeof (command_params));
  const MacronixInstruction & entry = g_instruction_map[command_type];

  ASSERT_TEST (command_type < (sizeof (g_instruction_map) / sizeof (MacronixInstruction)));
  ASSERT_DEV (entry.m_command_id == command_type);

  command_params.m_mode = entry.m_command_mode;
  command_params.m_instruction_type = entry.m_instruction_type;
  command_params.m_instruction = entry.m_instruction_value;
  command_params.m_dummy_cycles = (entry.m_dummy_cycles == 0xFFU) ? default_dummy_cycles : entry.m_dummy_cycles;
}

