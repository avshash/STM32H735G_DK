#ifndef _TYPE_MODULE_ACTION_H_
#define _TYPE_MODULE_ACTION_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeModuleAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeModuleAction
{
  MODULE_ACTION_OPEN,
  MODULE_ACTION_CONFIGURE_PLL_CLOCKS,
  MODULE_ACTION_START_PLL_CLOCKS,
  MODULE_ACTION_ACTIVATE_GPIO,
  MODULE_ACTION_START_MDMA,
  MODULE_ACTION_START,
  MODULE_ACTION_TICK
};

#endif  // _TYPE_MODULE_ACTION_H_
