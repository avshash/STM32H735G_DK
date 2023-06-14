#ifndef _DISC735_MODULE_MANAGER_FLASH_H_
#define _DISC735_MODULE_MANAGER_FLASH_H_

#include "ModuleManagerFlash.h"
#include "FlashManagerMacronix25LM.h"
#include "PlatformOctoSpi1.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerFlash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735ModuleManagerFlash : public ModuleManagerFlash
{
public:
  Disc735ModuleManagerFlash ();

private:
  PlatformOctoSpi1 m_physical;
  MacronixParams m_macronix_params;
  FlashManagerMacronix25LM m_flash_manager;
};

typedef Disc735ModuleManagerFlash PlatformModuleManagerFlash;

#endif  // _DISC735_MODULE_MANAGER_FLASH_H_

