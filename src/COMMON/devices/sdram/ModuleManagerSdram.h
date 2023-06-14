#ifndef _MODULE_MANAGER_SDRAM_H_
#define _MODULE_MANAGER_SDRAM_H_

#include "ModuleManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdram
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerSdram : public ModuleManager
{
public:
  virtual uint32_t getMemoryBase () const = 0;
  virtual uint32_t getMemorySize () const = 0;

protected:
  ModuleManagerSdram ();

private:
  virtual void doAction (TypeModuleAction) final;

  virtual void sdramTick () = 0;
  virtual void activateGpio () = 0;
  virtual void initSdram () = 0;
};

#endif  // _MODULE_MANAGER_SDRAM_H_
