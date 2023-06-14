#ifndef _SPI8_ACTION_WRITE_H_
#define _SPI8_ACTION_WRITE_H_

#include "spi8_action.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8ActionWrite
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Spi8ActionWrite : public Spi8Action
{
public:
  Spi8ActionWrite (TypeSpi8Bank bank_type, TypeSpi8Lines lines_type);
  bool commitWrite (const uint32_t * data, uint16_t data_size, bool b_dtr_enabled = false);
};

#endif  // _SPI8_ACTION_WRITE_H_
