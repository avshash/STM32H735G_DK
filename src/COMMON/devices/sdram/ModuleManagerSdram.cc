#include "Common.h"
#include "ModuleManagerSdram.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdram
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerSdram::ModuleManagerSdram () :
  ModuleManager (MODULE_MANAGER_SDRAM)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerSdram
// @method:   doAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerSdram::doAction (TypeModuleAction action)
{
  switch (action)
  {
    case MODULE_ACTION_TICK:
      sdramTick ();
      break;

    case MODULE_ACTION_ACTIVATE_GPIO:
      activateGpio ();
      break;

    case MODULE_ACTION_START_MDMA:
      initSdram ();
      break;

    case MODULE_ACTION_OPEN:
    case MODULE_ACTION_CONFIGURE_PLL_CLOCKS:
    case MODULE_ACTION_START_PLL_CLOCKS:
    case MODULE_ACTION_START:
      break;
  }
}


