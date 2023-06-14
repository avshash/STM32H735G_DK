#include "Common.h"
#include "ModuleManagerSdramCypress.h"

#include "MemoryManager.h"
#include "OctoSpi.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerSdramCypress::ModuleManagerSdramCypress (OctoSpi & sdram_interface) :
  ModuleManagerSdram (),
  m_octo_spi (sdram_interface),
  m_sdram_size (0x00800000UL)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   sdramTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerSdramCypress::sdramTick ()
{
  m_octo_spi.verifyMemoryMappedGood ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   activateGpio
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerSdramCypress::activateGpio ()
{
  m_octo_spi.activateGpio ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   initSdram
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerSdramCypress::initSdram ()
{
  configureMemorySize ();
  setupChip ();
  configureOspi (true);
  configureMpu ();

  volatile uint32_t * base = (volatile uint32_t *) getMemoryBase ();
  *base = 0xDEADBEEFUL;
  ASSERT_CRITICAL (base[0] == 0xDEADBEEFUL);
  base[(m_sdram_size / 4) - 1] = 0xBEEFDEADUL;
  ASSERT_CRITICAL (base[(m_sdram_size / 4) - 1] == 0xBEEFDEADUL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   configureMemorySize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerSdramCypress::configureMemorySize ()
{
  configureOspi (false);

  uint32_t die0_id_register = m_octo_spi.readHyperbusRegister (0x00000000UL);
  ASSERT_CRITICAL ((die0_id_register & 0x0000C00FUL) == 0x00000001UL);

  // Setup device size according to id register value.
  unsigned int row_address_bit_count = 1 + ((die0_id_register >> 8) & 0x1F);
  unsigned int column_address_bit_count = 1 + ((die0_id_register >> 4) & 0x0F);
  unsigned int device_words_log_size = row_address_bit_count + column_address_bit_count;
  ASSERT_CRITICAL ((22 <= device_words_log_size) && (device_words_log_size <= 23));
  m_sdram_size = 0x01UL << (device_words_log_size + 1);

  configureOspi (false);

  if (22 < device_words_log_size)
  {
    // Verify die 1 id register.
    uint32_t die1_id_register = m_octo_spi.readHyperbusRegister (0x00800000UL);
    ASSERT_CRITICAL ((die1_id_register & 0x0000C00FUL) == 0x00004001UL);
    ASSERT_CRITICAL ((die1_id_register & 0x00003FF0UL) == (die1_id_register & 0x00003FF0UL));
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   configureOspi
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerSdramCypress::configureOspi (bool b_memory_mapped)
{
  OspiConfigurationParams params (OSPI_MEMORY_HYPERBUS);

  params.setParam (OSPI_CONFIGURATION_PARAM_DEVICE_LOG_SIZE, Common::log2int (m_sdram_size));
  params.setParam (OSPI_CONFIGURATION_PARAM_CLOCK_MAX_FREQUENCY, 100000000);
  params.setParam (OSPI_CONFIGURATION_PARAM_CHIP_SELECT_HIGH_TIME, 4);
  // Read-write recovery time is at least 40 ns.
  params.setParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_READ_WRITE_RECOVERY_TIME, 4);
  params.setParam (OSPI_CONFIGURATION_PARAM_CS_BOUNDARY_LOG_SIZE, 23);      // Single die is 8 MBytes.
  // Refresh rate is 4 us. Single clock cycle is about 0x02 us.
  params.setParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_REFRESH_RATE, 72);
  params.setParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_FIXED_LATENCY, 1);
  // Mdma is setup for word access (4 bytes).
  params.setParam (OSPI_CONFIGURATION_PARAM_FIFO_THRESHOLD_LEVEL, 4);

  if (b_memory_mapped)
  {
    params.setParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_MEMORY_MAPPED, 1);
    params.setParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_ACCESS_TIME, 4);
  }
  else
  {
    params.setParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_WRITE_ZERO_LATENCY, 1);
    params.setParam (OSPI_CONFIGURATION_PARAM_HYPERBUS_ACCESS_TIME, 6);
  }

  m_octo_spi.configure (params);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   setupChip
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerSdramCypress::setupChip ()
{
  // Setup configuration register for die 0.
  setupConfigurationRegister (0x00001000UL);

  if (0x00800000UL < m_sdram_size)
  {
    // Setup configuration register for die 1, if present.
    setupConfigurationRegister (0x00801000UL);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   setupConfigurationRegister
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerSdramCypress::setupConfigurationRegister (uint32_t register_address)
{
  uint32_t configuration_register = m_octo_spi.readHyperbusRegister (register_address);
  ASSERT_CRITICAL (configuration_register == 0x00008F1FUL);

  // We are running at low frequency. Set read latency (ACCESS_TIME) to 4 clock cycles.
  m_octo_spi.writeHyperbusRegister (register_address, 0x00008FFFUL);

  configuration_register = m_octo_spi.readHyperbusRegister (register_address);
  ASSERT_CRITICAL (configuration_register == 0x00008FFFUL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   getMemoryBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
ModuleManagerSdramCypress::getMemoryBase () const
{
  return m_octo_spi.getDirectMemoryBase ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @method:   getMemorySize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
ModuleManagerSdramCypress::getMemorySize () const
{
  return m_sdram_size;
}

