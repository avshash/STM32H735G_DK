#ifndef _DISC735_MODULE_MANAGER_SDRAM_CYPRESS_H_
#define _DISC735_MODULE_MANAGER_SDRAM_CYPRESS_H_

#include "ModuleManagerSdramCypress.h"
#include "PlatformOctoSpi2.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerSdramCypress
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735ModuleManagerSdramCypress : public ModuleManagerSdramCypress
{
public:
  Disc735ModuleManagerSdramCypress ();

private:
  virtual void configureMpu () final;

  PlatformOctoSpi2 m_physical;
};

typedef Disc735ModuleManagerSdramCypress PlatformModuleManagerSdram;

#endif  // _DISC735_MODULE_MANAGER_SDRAM_CYPRESS_H_

