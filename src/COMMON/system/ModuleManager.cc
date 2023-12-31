#include "Common.h"
#include "ModuleManager.h"

#include "Main.h"
#include "ModuleManagerInterrupts.h"
#include "ModuleManagerClockControl.h"
#include "ModuleManagerLeds.h"
#include "ModuleManagerLcd.h"
#include "ModuleManagerFlash.h"
#include "ModuleManagerSdram.h"
#include "ModuleManagerUsb.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManager::ModuleManager (TypeModuleManager module_type)
{
  Main::registerModule (this, module_type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
// @method:   testModuleActive
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
ModuleManager::testModuleActive (TypeModuleManager manager_type)
{
  ModuleManager * module_manager = Main::getModuleManager (manager_type);

  return (module_manager != NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
// @method:   getInterrupts
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerInterrupts &
ModuleManager::getInterrupts ()
{
  ModuleManager * manager_base = Main::getModuleManager (MODULE_MANAGER_INTERRUPTS);
  ASSERT_DEV (manager_base != NULL);

  ModuleManagerInterrupts * interrupts_manager = static_cast <ModuleManagerInterrupts *> (manager_base);
  return *interrupts_manager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
// @method:   getClockControl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerClockControl &
ModuleManager::getClockControl ()
{
  ModuleManager * manager_base = Main::getModuleManager (MODULE_MANAGER_CLOCK_CONTROL);
  ASSERT_DEV (manager_base != NULL);

  ModuleManagerClockControl * clock_control_manager = static_cast <ModuleManagerClockControl *> (manager_base);
  return *clock_control_manager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
// @method:   getLeds
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerLeds &
ModuleManager::getLeds ()
{
  ModuleManager * manager_base = Main::getModuleManager (MODULE_MANAGER_LEDS);
  ASSERT_DEV (manager_base != NULL);

  ModuleManagerLeds * leds_manager = static_cast <ModuleManagerLeds *> (manager_base);
  return *leds_manager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
// @method:   getLcd
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerLcd &
ModuleManager::getLcd ()
{
  ModuleManager * manager_base = Main::getModuleManager (MODULE_MANAGER_LCD);
  ASSERT_DEV (manager_base != NULL);

  ModuleManagerLcd * lcd_manager = static_cast <ModuleManagerLcd *> (manager_base);
  return *lcd_manager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
// @method:   getFlash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerFlash &
ModuleManager::getFlash ()
{
  ModuleManager * manager_base = Main::getModuleManager (MODULE_MANAGER_FLASH);
  ASSERT_DEV (manager_base != NULL);

  ModuleManagerFlash * flash_manager = static_cast <ModuleManagerFlash *> (manager_base);
  return *flash_manager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
// @method:   getSdram
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerSdram &
ModuleManager::getSdram ()
{
  ModuleManager * manager_base = Main::getModuleManager (MODULE_MANAGER_SDRAM);
  ASSERT_DEV (manager_base != NULL);

  ModuleManagerSdram * sdram_manager = static_cast <ModuleManagerSdram *> (manager_base);
  return *sdram_manager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManager
// @method:   getUsb
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerUsb &
ModuleManager::getUsb ()
{
  ModuleManager * manager_base = Main::getModuleManager (MODULE_MANAGER_USB);
  ASSERT_DEV (manager_base != NULL);

  ModuleManagerUsb * usb_manager = static_cast <ModuleManagerUsb *> (manager_base);
  return *usb_manager;
}

