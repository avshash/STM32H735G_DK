#include "Common.h"
#include "spi8_init_params.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8InitParams
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Spi8InitParams::Spi8InitParams (TypeSpi8Configuration spi8_configuration, TypeSpi8Memory memory_type,
                                uint32_t device_bytes_count, uint16_t chip_select_high,
                                uint16_t clock_prescaler, bool b_dhqc_active)
: m_spi8_configuration (spi8_configuration),
  m_memory_type (memory_type),
  m_device_bytes_count (device_bytes_count),
  m_chip_select_high (chip_select_high),
  m_clock_prescaler (clock_prescaler),
  m_dhqc_active (b_dhqc_active)
{
  // Verify that device size is a power of 2.
  ASSERT_DEV (((device_bytes_count - 1) & device_bytes_count) == 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8InitParams
// @method:   configuration
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeSpi8Configuration
Spi8InitParams::configuration () const
{
  return m_spi8_configuration;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8InitParams
// @method:   bank
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeSpi8Bank
Spi8InitParams::bank () const
{
  switch (m_spi8_configuration)
  {
    case SPI8_CONFIGURATION_1_A:
      return SPI8_BANK_1;
  }

  ASSERT_DEV (false);
  return SPI8_BANK_1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8InitParams
// @method:   memoryType
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeSpi8Memory
Spi8InitParams::memoryType () const
{
  return m_memory_type;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8InitParams
// @method:   deviceShiftSize
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t
Spi8InitParams::deviceShiftSize () const
{
  uint16_t shift_size = 0;
  uint32_t bytes_count = 1;
  while (bytes_count < m_device_bytes_count)
  {
    bytes_count = bytes_count << 1;
    shift_size++;
  }

  return shift_size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8InitParams
// @method:   chipSelectHigh
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t
Spi8InitParams::chipSelectHigh () const
{
  return m_chip_select_high;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8InitParams
// @method:   clockPrescaler
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t
Spi8InitParams::clockPrescaler () const
{
  return m_clock_prescaler;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8InitParams
// @method:   dhqcActive
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
Spi8InitParams::dhqcActive () const
{
  return m_dhqc_active;
}

