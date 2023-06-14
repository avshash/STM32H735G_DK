#ifndef _FLASH_MANAGER_MACRONIX_25LM_H_
#define _FLASH_MANAGER_MACRONIX_25LM_H_

#include "FlashManager.h"
#include "MacronixParams.h"
#include "MacronixInterface.h"
#include "MacronixRegisters.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManagerMacronix25LM
// @dscr:     A serial NOR flash memory, accessed by the OSPI protocol.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FlashManagerMacronix25LM : public FlashManager
{
public:
  FlashManagerMacronix25LM (OctoSpi & flash_interface, const MacronixParams &);

private:
  virtual void doAction (TypeModuleAction) final;
  virtual uint32_t getFlashParam (TypeFlashParam param) const final;
  virtual void resetMemoryRange (uint32_t range_start, uint32_t range_limit) final;
  virtual void writeData (uint32_t address, const uint8_t * buffer, uint32_t bytes_count) final;
  virtual void readData (uint32_t address, uint8_t * buffer, uint32_t bytes_count) final;

  void configureMacronix ();
  uint32_t writePage (uint32_t address, const uint8_t * buffer, uint32_t buffer_size);
  bool testSectorReset (uint32_t sector_base);
  bool testPageData (uint32_t address, const uint8_t * buffer, uint32_t buffer_size);

  const MacronixParams & m_macronix_params;
  MacronixInterface m_flash_interface;
  MacronixRegisters m_configuration;
};

#endif  // _FLASH_MANAGER_MACRONIX_25LM_H_
