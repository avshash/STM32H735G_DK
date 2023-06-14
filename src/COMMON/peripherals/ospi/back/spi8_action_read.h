#ifndef _SPI8_ACTION_READ_H_
#define _SPI8_ACTION_READ_H_

#include "spi8_action.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Spi8ActionRead
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Spi8ActionRead : public Spi8Action
{
public:
  Spi8ActionRead (TypeSpi8Bank bank_type, TypeSpi8Lines lines_type);

  bool commitRead (uint32_t * data, uint16_t data_size, bool b_dtr_enabled = false);
};

#endif  // _SPI8_ACTION_READ_H_
