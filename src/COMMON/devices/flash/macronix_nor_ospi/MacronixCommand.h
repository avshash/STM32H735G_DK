#ifndef _MACRONIX_COMMAND_H_
#define _MACRONIX_COMMAND_H_

#include "OspiCommandParams.h"
#include "TypeMacronixCommand.h"

class MacronixInterface;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixCommand
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MacronixCommand
{
public:
  MacronixCommand (MacronixInterface &, TypeMacronixCommand);

  void configureAddress (uint32_t address);
  uint32_t writeData (const uint8_t * buffer, uint32_t requested_bytes_count);
  uint32_t readData (uint8_t * buffer, uint32_t requested_bytes_count);
  bool testIsSpi () const;

private:
  MacronixInterface & m_interface;
  OspiCommandParams m_command;
};

#endif  // _MACRONIX_COMMAND_H_
