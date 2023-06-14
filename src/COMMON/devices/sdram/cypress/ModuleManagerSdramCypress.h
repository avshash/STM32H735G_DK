#ifndef _MODULE_MANAGER_SDRAM_CYPRESS_H_
#define _MODULE_MANAGER_SDRAM_CYPRESS_H_

#include "ModuleManagerSdram.h"

class OctoSpi;
class MemoryManager;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdramCypress
// @dscr:     A serial sdram memory, accessed by the OSPI protocol.
// @note:     Follows infineon (cypress) documentation for S70KL1281.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerSdramCypress : public ModuleManagerSdram
{
public:
  ModuleManagerSdramCypress (OctoSpi & flash_interface);

protected:
  virtual uint32_t getMemoryBase () const final;
  virtual uint32_t getMemorySize () const final;

private:
  virtual void sdramTick () final;
  virtual void activateGpio () final;
  virtual void initSdram () final;

  void configureOspi (bool b_memory_mapped);
  void configureMemorySize ();
  void setupChip ();
  void setupConfigurationRegister (uint32_t register_address);

  virtual void configureMpu () = 0;

  OctoSpi & m_octo_spi;
  uint32_t m_sdram_size;
};

#endif  // _MODULE_MANAGER_SDRAM_CYPRESS_H_
