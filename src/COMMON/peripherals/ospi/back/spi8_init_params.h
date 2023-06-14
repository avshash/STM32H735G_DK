#ifndef _SPI8_INIT_PARAMS_H_
#define _SPI8_INIT_PARAMS_H_

#include "type_spi8_configuration.h"
#include "type_spi8_action.h"
#include "type_spi8_bank.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeSpi8Memory
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeSpi8Memory
{
  SPI8_MEMORY_MACRONIX
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8InitParams
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Spi8InitParams
{
public:
  Spi8InitParams (TypeSpi8Configuration, TypeSpi8Memory, uint32_t device_bytes_count, uint16_t chip_select_high, 
                  uint16_t clock_prescaler, bool b_dhqc_active);

  TypeSpi8Configuration configuration () const;
  TypeSpi8Bank bank () const;
  TypeSpi8Memory memoryType () const;
  uint16_t deviceShiftSize () const;
  uint16_t chipSelectHigh () const;
  uint16_t clockPrescaler () const;
  bool dhqcActive () const;

private:
  const TypeSpi8Configuration m_spi8_configuration;
  const TypeSpi8Memory m_memory_type;
  const uint32_t m_device_bytes_count;
  const uint16_t m_chip_select_high;
  const uint16_t m_clock_prescaler;
  const bool m_dhqc_active;
};

#endif  // _SPI8_INIT_PARAMS_H_
