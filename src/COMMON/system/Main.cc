#include "Common.h"
#include "Main.h"

#include "ModuleManager.h"
#include "TextSingleLine.h"
#include "Timer.h"
#include "SystemTime.h"
#include <cstring>

Main * Main::m_singleton;
ModuleManager * Main::m_managers_array[MODULE_MANAGERS_COUNT];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Main
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Main::Main () :
  m_actions_count (0),
  m_system_time ()
{
  // All global variables are initialized to 0 by 'resetHandler'.
  ASSERT_TEST (m_singleton == NULL);

  m_singleton = this;
  memset (m_managers_array, 0, sizeof (m_managers_array));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Main
// @method:   registerModule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Main::registerModule (ModuleManager * manager_ptr, TypeModuleManager manager_type)
{
  ASSERT_TEST (manager_type < MODULE_MANAGERS_COUNT);
  ASSERT_TEST (m_managers_array[manager_type] == NULL);

  m_managers_array[manager_type] = manager_ptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Main
// @method:   getModuleManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManager *
Main::getModuleManager (TypeModuleManager manager_type)
{
  ASSERT_TEST (manager_type < MODULE_MANAGERS_COUNT);

  return m_managers_array[manager_type];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Main
// @method:   main
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Main::main (uint32_t static_size)
{
  while (1)
  {
    registerPhaseStarted (m_actions_count);
    modulesTick ();
    advancePhase ();

    if (m_actions_count == MODULE_ACTION_TICK)
    {
      if (ModuleManager::testModuleActive (MODULE_MANAGER_LCD))
      {
        TextSingleLine text_single_line;
        text_single_line << "Size of static objects is " << static_size << ".";
      }
    }

    ASSERT_TEMP (SystemTime::getSystemMilliTime () < 0x3FFF0000UL);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Main
// @method:   modulesTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Main::modulesTick ()
{
  bool b_tick_action = (MODULE_ACTION_TICK <= m_actions_count);
  TypeModuleAction action = b_tick_action ? MODULE_ACTION_TICK : (TypeModuleAction) m_actions_count;

  for (uint32_t module_type = MODULE_MANAGER_PHYSICAL; module_type < MODULE_MANAGERS_COUNT; module_type++)
  {
    ModuleManager * cur_module = m_managers_array[module_type];
    if (cur_module != NULL)
    {
      cur_module->doAction (action);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Main
// @method:   advancePhase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Main::advancePhase ()
{
  if (m_actions_count == 0)
  {
    // Allow for peripherals wakeup.
    Common::waitMicro (100000);
  }

  if (m_actions_count < MODULE_ACTION_TICK)
  {
    // Advance system clock after each initialization phase.
    Common::waitMicro (1000);
  }

  m_actions_count++;
}


