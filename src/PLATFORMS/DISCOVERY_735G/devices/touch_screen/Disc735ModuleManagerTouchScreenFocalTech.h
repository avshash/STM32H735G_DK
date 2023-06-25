#ifndef _DISC735_MODULE_MANAGER_TOUCH_SCREEN_FOCAL_TECH_H_
#define _DISC735_MODULE_MANAGER_TOUCH_SCREEN_FOCAL_TECH_H_

#include "ModuleManagerTouchScreenFocalTech.h"
#include "PlatformI2cMasterManager4.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerTouchScreenFocalTech
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735ModuleManagerTouchScreenFocalTech : public ModuleManagerTouchScreenFocalTech
{
public:
  Disc735ModuleManagerTouchScreenFocalTech (ListenerTouchScreen &);

private:
   PlatformI2cMasterManager4 m_interface;
};

typedef Disc735ModuleManagerTouchScreenFocalTech PlatformModuleManagerTouchScreen;

#endif  // _DISC735_MODULE_MANAGER_TOUCH_SCREEN_FOCAL_TECH_H_

