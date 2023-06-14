#ifndef _MACRONIX_ACTION_WRITE_H_
#define _MACRONIX_ACTION_WRITE_H_

#include "MacronixCommand.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixActionWrite
// @note:     When 'dummy_cycles' is '0', mode is SPI. Otherwise, mode is OPI.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MacronixActionWrite
{
public:
  MacronixActionWrite (MacronixInterface &, TypeMacronixCommand);

  unsigned int writeData (const uint8_t * buffer = NULL, unsigned int requested_bytes_count = 0);
  unsigned int writeData (uint32_t address, const uint8_t * buffer = NULL, unsigned int requested_bytes_count = 0);

private:
  bool testWriteSuccess ();

  MacronixCommand m_action;
  MacronixCommand m_wren;
  MacronixCommand m_rdsr;
};

#endif  // _MACRONIX_ACTION_WRITE_H_
