#include "Common.h"
#include "MacronixRegisters.h"

#include "MacronixActionWrite.h"
#include "MacronixInterface.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MacronixRegisters::MacronixRegisters (MacronixInterface & interface) :
  m_interface (interface)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   setupStrMode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixRegisters::setupStrMode (uint32_t chip_id_target)
{
  // Verify chip is in SPI mode.
  if (!testMacronixModeSpi (chip_id_target))
  {
    if (!testMacronixModeStr (chip_id_target))
    {
      ASSERT_CRITICAL (false);
    }

    // Default mode should be SPI, not STR.
    ASSERT_TEST (false);

    // Configure mode from STR to SPI.
    uint8_t register_base = strReadConfigurationRegister2 (0x00000000UL) & 0x00FC;
    strWriteConfigurationRegister2 (0x00000000UL, register_base);
  }

  setupRegisters ();

  // Configure mode from SPI to STR.
  uint8_t register_base = spiReadConfigurationRegister2 (0x00000000UL) & 0x00FC;
  spiWriteConfigurationRegister2 (0x00000000UL, (register_base | 0x01));

  ASSERT_CRITICAL (testMacronixModeStr (chip_id_target));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   testMacronixMode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
MacronixRegisters::testMacronixModeSpi (uint32_t chip_id_target)
{
  uint32_t chip_id_actual = 0;

  MacronixCommand test_spi (m_interface, MACRONIX_COMMAND_SPI_RDID);
  test_spi.readData ((uint8_t *) &chip_id_actual, 3);

  return (chip_id_target == chip_id_actual);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   testMacronixModeStr
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
MacronixRegisters::testMacronixModeStr (uint32_t chip_id_target)
{
  uint32_t chip_id_actual = 0;

  MacronixCommand test_spi (m_interface, MACRONIX_COMMAND_OPI_RDID);
  test_spi.configureAddress (0x00000000UL);
  test_spi.readData ((uint8_t *) &chip_id_actual, 3);

  return (chip_id_target == chip_id_actual);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   setupRegisters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixRegisters::setupRegisters ()
{
  setupDummyCycles ();

  // DQS setup.
  uint8_t dqs_register = spiReadConfigurationRegister2 (0x00000200UL);
  if ((dqs_register & 0x03U) != 0)
  {
    ASSERT_TEST (false);
    spiWriteConfigurationRegister2 (0x00000200UL, (dqs_register & 0x00FC));
  }

  verifySecurityRegister ();
  spiSetMemoryProtection (true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   spiReadConfigurationRegister2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t
MacronixRegisters::spiReadConfigurationRegister2 (uint32_t address)
{
  uint8_t ret_val = 0;

  MacronixCommand spi_rdcr2 (m_interface, MACRONIX_COMMAND_SPI_RDCR2);
  spi_rdcr2.configureAddress (address);

  spi_rdcr2.readData (&ret_val, 1);

  return ret_val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   strReadConfigurationRegister2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t
MacronixRegisters::strReadConfigurationRegister2 (uint32_t address)
{
  uint8_t ret_val = 0;

  MacronixCommand str_rdcr2 (m_interface, MACRONIX_COMMAND_OPI_RDCR2);
  str_rdcr2.configureAddress (address);

  str_rdcr2.readData (&ret_val, 1);

  return ret_val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   spiWriteConfigurationRegister2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixRegisters::spiWriteConfigurationRegister2 (uint32_t address, uint8_t value)
{
  MacronixActionWrite spi_wrcr2 (m_interface, MACRONIX_COMMAND_SPI_WRCR2);
  spi_wrcr2.writeData (address, &value, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   strWriteConfigurationRegister2
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixRegisters::strWriteConfigurationRegister2 (uint32_t address, uint8_t value)
{
  MacronixActionWrite str_wrcr2 (m_interface, MACRONIX_COMMAND_OPI_WRCR2);
  str_wrcr2.writeData (address, &value, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   setupDummyCycles
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixRegisters::setupDummyCycles ()
{
  uint8_t register_value = 0x00;

  switch (m_interface.getDummyCycles ())
  {
    case 20:
      register_value = 0x00;
      break;

    case 18:
      register_value = 0x01;
      break;

    case 16:
      register_value = 0x02;
      break;

    case 14:
      register_value = 0x03;
      break;

    case 12:
      register_value = 0x04;
      break;

    case 10:
      register_value = 0x05;
      break;

    case 8:
      register_value = 0x06;
      break;

    case 6:
      register_value = 0x07;
      break;

    default:
      ASSERT_CRITICAL (false);
      break;
  }

  uint8_t register_base = spiReadConfigurationRegister2 (0x00000300UL) & 0x00F8;

  spiWriteConfigurationRegister2 (0x00000300UL, register_base | register_value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   verifySecurityRegister
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixRegisters::verifySecurityRegister ()
{
  uint8_t security_register = 0;

  MacronixCommand spi_rdscur (m_interface, MACRONIX_COMMAND_SPI_RDSCUR);

  spi_rdscur.readData (&security_register, 1);

  // Verify that configuration is locked under factory defaults.
  ASSERT_TEST ((security_register & 0x0083) == 0x00);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   spiSetMemoryProtection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixRegisters::spiSetMemoryProtection (bool b_enable)
{
  uint8_t status_register = 0;

  MacronixCommand spi_rdsr (m_interface, MACRONIX_COMMAND_SPI_RDSR);

  spi_rdsr.readData (&status_register, 1);

  uint8_t target_value = b_enable ? 0x3C : 0x00;

  if ((status_register & 0x3C) != target_value)
  {
    status_register = (status_register & 0xC3) | target_value;

    MacronixActionWrite spi_wrsr (m_interface, MACRONIX_COMMAND_SPI_WRSR);
    spi_wrsr.writeData (&status_register, 1);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @method:   strSetMemoryProtection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MacronixRegisters::strSetMemoryProtection (bool b_enable)
{
  uint8_t status_register = 0;

  MacronixCommand str_rdsr (m_interface, MACRONIX_COMMAND_OPI_RDSR);
  str_rdsr.configureAddress (0x00000000UL);

  str_rdsr.readData (&status_register, 1);

  uint8_t target_value = b_enable ? 0x3C : 0x00;

  if ((status_register & 0x3C) != target_value)
  {
    status_register = (status_register & 0xC3) | target_value;

    MacronixActionWrite str_wrsr (m_interface, MACRONIX_COMMAND_OPI_WRSR_WRCR);
    str_wrsr.writeData (0x00000000UL, &status_register, 1);
  }
}

