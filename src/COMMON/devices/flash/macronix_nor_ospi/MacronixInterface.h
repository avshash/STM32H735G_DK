#ifndef _MACRONIX_INTERFACE_H_
#define _MACRONIX_INTERFACE_H_

#include "OctoSpi.h"

class MacronixParams;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixInterface
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MacronixInterface
{
public:
  MacronixInterface (OctoSpi & ospi_interface, const MacronixParams &);

  void activateGpio ();
  void enableOspi (const MacronixParams &);
  unsigned int writeData (const OspiCommandParams &);
  unsigned int readData (OspiCommandParams &);
  uint8_t getDummyCycles () const;

private:
  OctoSpi & m_ospi_interface;
  uint8_t m_dummy_cycles;
};

#endif  // _MACRONIX_INTERFACE_H_
