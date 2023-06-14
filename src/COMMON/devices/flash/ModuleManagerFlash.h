#ifndef _MODULE_MANAGER_FLASH_H_
#define _MODULE_MANAGER_FLASH_H_

#include "ModuleManager.h"

class Flash;
class FlashManager;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerFlash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerFlash : public ModuleManager
{
public:
  Flash & getFlashInterface ();

protected:
  ModuleManagerFlash (FlashManager &);
  
private:
  virtual void doAction (TypeModuleAction) final;

  FlashManager & m_flash_manager;
};

#endif  // _MODULE_MANAGER_FLASH_H_
