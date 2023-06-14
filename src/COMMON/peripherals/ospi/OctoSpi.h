#ifndef _OCTO_SPI_H_
#define _OCTO_SPI_H_

#include "TypeOspiMemory.h"
#include "OspiCommandParams.h"
#include "OspiConfigurationParams.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    OctoSpi
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OctoSpi
{
public:
  virtual void activateGpio () = 0;
  virtual void configure (const OspiConfigurationParams & params) = 0;
  virtual unsigned int commitWriteCommand (const struct OspiCommandParams &) = 0;
  virtual unsigned int commitReadCommand (struct OspiCommandParams &) = 0;

  // Hyperbus interface for memory mapped mode.
  virtual uint32_t getDirectMemoryBase () const = 0;
  virtual uint32_t readHyperbusRegister (uint32_t address) = 0;
  virtual void writeHyperbusRegister (uint32_t address, uint32_t register_value) = 0;
  virtual void verifyMemoryMappedGood () = 0;
};

#endif  // _OCTO_SPI_H_
