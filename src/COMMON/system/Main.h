#ifndef _MAIN_H_
#define _MAIN_H_

#include "SystemTime.h"
#include "TypeModuleManager.h"
#include "TypeModuleAction.h"

class ModuleManager;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Main
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Main
{
public:
  void main ();

  static void registerModule (ModuleManager *, TypeModuleManager);
  static ModuleManager * getModuleManager (TypeModuleManager);

protected:
  Main ();

private:
  void modulesTick ();
  void advancePhase ();
  virtual void registerPhaseStarted (uint32_t actions_count) = 0;

  uint32_t m_actions_count;
  SystemTime m_system_time;

  static Main * m_singleton;
  static ModuleManager * m_managers_array[MODULE_MANAGERS_COUNT];
};

#endif  // _MAIN_H_
