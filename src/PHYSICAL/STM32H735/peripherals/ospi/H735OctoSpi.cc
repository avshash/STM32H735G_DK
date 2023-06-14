#include "Common.h"
#include "H735OctoSpi.h"

#include "ModuleManagerClockControl.h"
#include "PhysicalRccRegisters.h"
#include "PhysicalCriticalSection.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735OctoSpi::H735OctoSpi (H735OctoSpiRegisters & octo_spi_registers) :
  m_octo_spi_registers (octo_spi_registers)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   configure
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::configure (const OspiConfigurationParams & params)
{
  bool b_memory_mapped = (params.getParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_MEMORY_MAPPED) != 0);

  // Disable spi module before configuration.
  m_octo_spi_registers.CR = 0x00000000UL;

  enableSpiClock ();
  configureDcr1 (params.getType (),
                 b_memory_mapped,
                 params.getParam (OSPI_CONFIGURATION_PARAM_DEVICE_LOG_SIZE),
                 params.getParam (OSPI_CONFIGURATION_PARAM_CHIP_SELECT_HIGH_TIME));
  configureDcr2 (params.getParam (OSPI_CONFIGURATION_PARAM_CLOCK_MAX_FREQUENCY));
  configureDcr3 (params.getParam (OSPI_CONFIGURATION_PARAM_CS_BOUNDARY_LOG_SIZE));
  configureDcr4 (params.getParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_REFRESH_RATE));

  bool b_dhqc_active = (params.getType () == OSPI_MEMORY_HYPERBUS);
  configureTcr (b_dhqc_active);
  configureHlcr (params.getParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_READ_WRITE_RECOVERY_TIME),
                 params.getParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_ACCESS_TIME),
                 (params.getParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_FIXED_LATENCY) != 0),
                 (params.getParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_WRITE_ZERO_LATENCY) != 0));

  configureCr (b_memory_mapped, params.getParam (OSPI_CONFIGURATION_PARAM_FIFO_THRESHOLD_LEVEL));

  if (b_memory_mapped)
  {
    configureHyperbusCcr ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   configureDcr1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::configureDcr1 (TypeOspiMemory memory_type, bool b_memory_mapped,
                            uint32_t device_log_size, uint32_t chip_select_high_time)
{
  ASSERT_CRITICAL ((!b_memory_mapped) || (memory_type == OSPI_MEMORY_HYPERBUS));

  uint32_t type_mask = 0;
  switch (memory_type)
  {
    case OSPI_MEMORY_MICRON:
      type_mask = 0x00000000UL;
      // Needs testing.
      ASSERT_CRITICAL (false);
      break;

    case OSPI_MEMORY_MACRONIX:
      type_mask = 0x01000000UL;
      break;

    case OSPI_MEMORY_STANDARD:
      type_mask = 0x02000000UL;
      // Needs testing.
      ASSERT_CRITICAL (false);
      break;

    case OSPI_MEMORY_MACRONIX_RAM:
      type_mask = 0x03000000UL;
      // Needs testing.
      ASSERT_CRITICAL (false);
      break;

    case OSPI_MEMORY_HYPERBUS:
      // Initialize register hyperbus for configuration. Memory hyperbus is enabled when setting up memory mapped mode.
      type_mask = b_memory_mapped ? 0x04000000UL : 0x05000000UL;
      break;
  }

  ASSERT_TEST ((0 < device_log_size) && (device_log_size < 32));
  uint32_t size_mask = (device_log_size - 1) << 16;

  ASSERT_TEST ((0 < chip_select_high_time) && (chip_select_high_time < 64));
  uint32_t csht_mask = (chip_select_high_time - 1) << 8;

  m_octo_spi_registers.DCR1 = type_mask | size_mask | csht_mask;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   configureDcr2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::configureDcr2 (uint32_t max_clock_frequency)
{
  ModuleManagerClockControl & clock_control = ModuleManager::getClockControl ();

  // Verify that SPI clock source is rcc_hclk3 clock.
  ASSERT_TEST ((RCC_REGISTERS.D1CCIPR & 0x00000030UL) == 0);
  uint32_t source_frequency = clock_control.getClockFrequency (CLOCK_BUS_HCLK);

  uint32_t clock_divider = 1;
  while (max_clock_frequency < (source_frequency / clock_divider))
  {
    clock_divider++;
  }
  ASSERT_TEST (clock_divider < 256);

  m_octo_spi_registers.DCR2 = clock_divider - 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   configureDcr3
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::configureDcr3 (uint32_t cs_bound_log_size)
{
  ASSERT_TEST (cs_bound_log_size < 32);
  m_octo_spi_registers.DCR3 = cs_bound_log_size << 16;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   configureDcr4
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::configureDcr4 (uint32_t refresh_rate)
{
  m_octo_spi_registers.DCR4 = refresh_rate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   configureTcr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::configureTcr (uint32_t b_dhqc_active)
{
  uint32_t dhqc_mask = (b_dhqc_active != 0) ? 0x10000000UL : 0;

  m_octo_spi_registers.TCR = dhqc_mask;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   configureHlcr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::configureHlcr (uint32_t rw_recovery_time, uint32_t access_time,
                            bool b_fixed_latency, bool b_write_zero_latency)
{
  ASSERT_TEST (rw_recovery_time < 0x100UL);
  ASSERT_TEST (access_time < 0x100UL);
  uint32_t write_zero_latency = b_write_zero_latency ? 0x02 : 0;
  uint32_t latency_value = b_fixed_latency ? 0x01 : 0;

  m_octo_spi_registers.HLCR = (rw_recovery_time << 16) | (access_time << 8) | latency_value | write_zero_latency;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   configureCr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::configureCr (bool b_memory_mapped, uint32_t fifo_thresthod_level)
{
  // 'fifo_thresthod_level' default value is '1'.
  fifo_thresthod_level = MAX (1, fifo_thresthod_level);
  ASSERT_CRITICAL (fifo_thresthod_level <= 0x20);

  uint32_t fifo_value = (fifo_thresthod_level - 1) << 8;
  uint32_t memory_mapped = b_memory_mapped ? 0x30000000UL : 0;

  m_octo_spi_registers.CR = 0x00000001UL | fifo_value | memory_mapped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   configureHyperbusCcr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::configureHyperbusCcr ()
{
  // CCR register has DQS enabled, DDTR enabled, data on 8 lines, address of 4 bytes on 8 lines.
  m_octo_spi_registers.CCR = 0x2C003C00UL;
  m_octo_spi_registers.WCCR = 0x2C003C00UL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   enableSpiClock
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::enableSpiClock ()
{
  ASSERT_TEST (((&m_octo_spi_registers) == (&OCTO_SPI_1_REGISTERS)) ||
               ((&m_octo_spi_registers) == (&OCTO_SPI_2_REGISTERS)));

  bool b_spi_1 = ((&m_octo_spi_registers) == (&OCTO_SPI_1_REGISTERS));

  uint32_t enable_mask = b_spi_1 ? 0x00004000UL : 0x00080000UL;
  RCC_REGISTERS.AHB3ENR = RCC_REGISTERS.AHB3ENR | enable_mask;
  while ((RCC_REGISTERS.AHB3ENR & enable_mask) == 0);

  uint32_t ahb3rstr = RCC_REGISTERS.AHB3RSTR;
  // Resets the OCTOSPIx block.
  RCC_REGISTERS.AHB3RSTR = ahb3rstr | enable_mask;
  // Releases the OCTOSPIx block reset.
  RCC_REGISTERS.AHB3RSTR = ahb3rstr & (~enable_mask);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   commitWriteCommand
// @note:     Can never fail.
// @return:   Actual number of bytes written to device.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int
H735OctoSpi::commitWriteCommand (const OspiCommandParams & message_params)
{
  // Set to indirect write mode.
  m_octo_spi_registers.CR = 0x00000001UL;
  setCommandParams (message_params);

  criticalWrite (message_params.m_data);

  // Write command can never fail.
  cleanupTransfer ();

  return dataSize ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   criticalWrite
// @note:     We must protect the write action from interrupts.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::criticalWrite (uint32_t const * buffer)
{
  volatile uint32_t & octo_spi_dr = m_octo_spi_registers.DR;
  const uint32_t * buffer_limit = buffer + ((dataSize ()  + 3) / 4);

  {
    // This is a short critical section. Disable interrupts.
    PhysicalCriticalSection critical_section;

    do
    {
      octo_spi_dr = *buffer;
      buffer++;
    } while (buffer < buffer_limit);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   commitReadCommand
// @return:   Number of bytes read on success, or a negative number of failure.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int
H735OctoSpi::commitReadCommand (OspiCommandParams & message_params)
{
  // Set to indirect read mode.
  m_octo_spi_registers.CR = 0x10000001UL;
  setCommandParams (message_params);

  readReply (message_params.m_data);

  cleanupTransfer ();

  return dataSize ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   getDirectMemoryBase
// @note:     Follows RM0468, section 2.3.2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735OctoSpi::getDirectMemoryBase () const
{
  if (&m_octo_spi_registers == &OCTO_SPI_2_REGISTERS)
  {
    return 0x70000000UL;
  }

  if (&m_octo_spi_registers == &OCTO_SPI_1_REGISTERS)
  {
    return 0x90000000UL;
  }

  ASSERT_CRITICAL (false);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   readHyperbusRegister
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735OctoSpi::readHyperbusRegister (uint32_t address)
{
  // Verify interface is configured to hyperbus register mode.
  ASSERT_CRITICAL ((m_octo_spi_registers.DCR1 & 0x07000000UL) == 0x05000000UL);
  // Verify that memory mapped mode is not active, and that interface is enabled.
  ASSERT_CRITICAL ((m_octo_spi_registers.CR & 0x20000005UL) == 0x00000001UL);
  ASSERT_TEST ((m_octo_spi_registers.SR & 0x20) == 0);

  // Setup indirect read mode.
  m_octo_spi_registers.CR = m_octo_spi_registers.CR | 0x10000000UL;

  configureHyperbusCcr ();

  // Set data length to 2 bytes.
  m_octo_spi_registers.DLR = 1;
  m_octo_spi_registers.AR = address;

  uint32_t ret_val = 0;
  readReply (&ret_val);
  cleanupTransfer ();

  return (ret_val & 0x0000FFFFUL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   writeHyperbusRegister
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::writeHyperbusRegister (uint32_t address, uint32_t register_value)
{
  ASSERT_TEST ((register_value & 0xFFFF0000UL) == 0);

  // Verify interface is configured to hyperbus register mode.
  ASSERT_CRITICAL ((m_octo_spi_registers.DCR1 & 0x07000000UL) == 0x05000000UL);
  // Verify that memory mapped mode is not active, and that interface is enabled.
  ASSERT_CRITICAL ((m_octo_spi_registers.CR & 0x20000005UL) == 0x00000001UL);
  ASSERT_TEST ((m_octo_spi_registers.SR & 0x20) == 0);

  // Setup indirect write mode.
  m_octo_spi_registers.CR = m_octo_spi_registers.CR & 0xEFFFFFFFUL;

  configureHyperbusCcr ();

  // Set data length to 2 bytes.
  m_octo_spi_registers.DLR = 1;
  m_octo_spi_registers.AR = address;

  criticalWrite (&register_value);
  cleanupTransfer ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   verifyMemoryMappedGood
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::verifyMemoryMappedGood ()
{
  uint32_t octo_spi_sr = m_octo_spi_registers.SR;
  ASSERT_CRITICAL ((octo_spi_sr & 0x00000031UL) == 0x00000020UL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   readReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::readReply (uint32_t * buffer)
{
  // Wait for either transfer complete, or for any error.
  uint32_t octo_spi_sr = 0;
  while (octo_spi_sr == 0)
  {
    octo_spi_sr = m_octo_spi_registers.SR & 0x00000013UL;
  }

  // Read data if transfer complete and no error registered.
  if (octo_spi_sr == 0x00000002UL)
  {
    volatile uint32_t & octo_spi_dr = m_octo_spi_registers.DR;
    const uint32_t * buffer_limit = buffer + ((dataSize () + 3) / 4);

    while (buffer < buffer_limit)
    {
      *buffer = octo_spi_dr;
      buffer++;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   cleanupTransfer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::cleanupTransfer ()
{
  // Wait for either transfer complete, or for any error.
  while ((m_octo_spi_registers.SR & 0x00000013UL) == 0);
  uint32_t octo_spi_sr = m_octo_spi_registers.SR;

  // Verify no unexpected transfer error (invalid address).
  ASSERT_TEST ((octo_spi_sr & 0x00000001UL) == 0);

  // Test for timeout, or invalid address.
  if ((octo_spi_sr & 0x00000011UL) != 0)
  {
    // Abort transfer.
    m_octo_spi_registers.CR = m_octo_spi_registers.CR | 0x00000002UL;
    while ((m_octo_spi_registers.CR & 0x00000002UL) != 0);

    // There should never be any transfer error.
    ASSERT_CRITICAL (false);
  }

  // Clear error bits in the status register (TOF and TCF), as well as the transfer complete flag.
  m_octo_spi_registers.FCR = 0x00000013UL;

  // Ignore the FIFO threshold flag, as it is set on write actions.
  ASSERT_TEST ((m_octo_spi_registers.SR & 0xFFFFFFFBUL) == 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   setCommandParams
// @dscr:     Configures the DLR register, followed by the TCR, and CCR registers (in that order).
// @dscr:     Initiate the action by writing to the IR register, followed by a write to the AR register (if relevent).
// @return:   Length of data to read / write.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::setCommandParams (const OspiCommandParams & message_params)
{
  // Limit bytes count to integrated fifo buffer size.
  int data_len = MIN (32, message_params.m_data_bytes_len);
  if (0 < data_len)
  {
    m_octo_spi_registers.DLR = data_len - 1;
  }

  // Configure dummy cycles.
  m_octo_spi_registers.TCR = (m_octo_spi_registers.TCR & 0xFFFFFFE0UL) | message_params.m_dummy_cycles;

  setCcrRegister (message_params);

  // Configure instruction. Always active.
  ASSERT_TEST (message_params.m_instruction_type != OSPI_INSTRUCTION_BYTES_0);
  m_octo_spi_registers.IR = message_params.m_instruction;

  // Configure address.
  if (message_params.m_address_type != OSPI_ADDRESS_BYTES_0)
  {
    m_octo_spi_registers.AR = message_params.m_address;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   setCcrRegister
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735OctoSpi::setCcrRegister (const OspiCommandParams & message_params)
{
  ASSERT_TEST ((m_octo_spi_registers.SR & 0x00000020UL) == 0);

  uint32_t ccr_register = 0;
  uint32_t lines_mask = 0;

  // Configure lines mask. Do now allow mixing of SPI and OPI modes in the same message.
  switch (message_params.m_mode)
  {
    case OSPI_MODE_SPI:
      lines_mask = 0x01;
      break;

    case OSPI_MODE_STR:
      lines_mask = 0x04;
      break;

    case OSPI_MODE_DISABLED:
    case OSPI_MODE_DTR:
    case OSPI_MODE_DTR_DQS:
      break;
  }
  ASSERT_TEST (lines_mask != 0);

  // Configure instruction params.
  switch (message_params.m_instruction_type)
  {
    case OSPI_INSTRUCTION_BYTES_0:
      break;

    case OSPI_INSTRUCTION_BYTES_1:
      ccr_register = ccr_register | lines_mask;
      break;

    case OSPI_INSTRUCTION_BYTES_2:
      ccr_register = ccr_register | 0x00000010UL | lines_mask;
      break;

    case OSPI_INSTRUCTION_BYTES_4:
      ccr_register = ccr_register | 0x00000030UL | lines_mask;
      break;
  }

  // Configure address params.
  switch (message_params.m_address_type)
  {
    case OSPI_ADDRESS_BYTES_0:
      break;

    case OSPI_ADDRESS_BYTES_3:
      ccr_register = ccr_register | 0x00002000UL | (lines_mask << 8);
      break;

    case OSPI_ADDRESS_BYTES_4:
      ccr_register = ccr_register | 0x00003000UL | (lines_mask << 8);
      break;
  }


  // Configure data params.
  if (0 < message_params.m_data_bytes_len)
  {
    ccr_register = ccr_register | (lines_mask << 24);
  }

  m_octo_spi_registers.CCR = ccr_register;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @method:   dataSize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int
H735OctoSpi::dataSize () const
{
  if ((m_octo_spi_registers.CCR & 0x07000000UL) == 0)
  {
    return 0;
  }

  return (m_octo_spi_registers.DLR + 1);
}

