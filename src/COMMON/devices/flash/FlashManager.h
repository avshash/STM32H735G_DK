#ifndef _FLASH_MANAGER_H_
#define _FLASH_MANAGER_H_

#include "Flash.h"
#include "TypeModuleAction.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FlashManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FlashManager : public Flash
{
public:
  virtual void doAction (TypeModuleAction) = 0;
};

#endif  // _FLASH_MANAGER_H_
