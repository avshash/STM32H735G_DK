#include "common.h"
#include "Target.h"

#include "FileWriter.h"
#include "Configuration.h"
#include "Module.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Target::Target (Library & parent, const QueueString & params) :
  m_target_library (parent),
  m_target_params (),
  m_active_modules ()
{
  m_target_params.enqueue (params);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
// @method:   destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Target::~Target ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
// @method:   activateTarget
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Target::activateTarget (Configuration & configuration, const String & target_name)
{
  m_target_params.enqueue ("TARGET_ACTIVE");
  m_target_params.enqueue (target_name);
  m_target_library.setTargetName (target_name);
  
  fprintf (stdout, "Activate target '%s'.\n", target_name.cStr ());
  activateModules (configuration, m_target_params);
  m_target_library.addLibraryParams (m_target_params);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
// @method:   activateModules
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Target::activateModules (Configuration & configuration, const QueueString & params_list)
{
  uint32_t params_count = params_list.queueSize ();
  for (uint32_t index = 0; index < params_count; index = index + 2)
  {
    if (params_list.getMember (index) == "MODULE_DEPEND")
    {
      const String & module_name = params_list.getMember (index + 1);
      activateModule (configuration, module_name);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
// @method:   activateModule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Target::activateModule (Configuration & configuration, const String & module_name)
{
  if (m_active_modules.testKey (module_name))
  {
    return;
  }

  m_active_modules.insertObject (module_name, NULL);

  if (!configuration.testObjectKey (module_name))
  {
    printError ("Missing module definitions '%s'.\n", module_name.cStr ());
    VERIFY_INPUT_VALID (false, "Depend module not defined.");
  }
  Object & module_object = configuration.getObject (module_name);
  VERIFY_INPUT_VALID (module_object.objectType () == OBJECT_MODULE, "Depend object is not a module.");
  Module & cur_module = static_cast <Module &> (module_object);

  cur_module.setModuleActive (configuration, module_name);

  const QueueString & module_params = cur_module.getModuleParams ();
  activateModules (configuration, module_params);
  m_target_params.enqueue (module_params);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
// @method:   writeTargetRule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Target::writeTargetRule (FileWriter & makefile) const
{
  uint32_t params_count = m_target_params.queueSize ();
  for (uint32_t index = 0; index < params_count; index = index + 2)
  {
    if (m_target_params.getMember (index) == "TARGET_ACTIVE")
    {
      makefile << m_target_params.getMember (index + 1) << ": \\\n";
    }

    if (m_target_params.getMember (index) == "MODULE_ACTIVE")
    {
      makefile << "\t" << m_target_params.getMember (index + 1) << " \\\n";
    }
  }

  makefile << "\n\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
// @method:   targetLibrary
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Library &
Target::targetLibrary ()
{
  return m_target_library;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
// @method:   getInputFile
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const QueueString &
Target::getTargetParams () const
{
  return m_target_params;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
// @method:   objectType
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeObject
Target::objectType () const
{
  return OBJECT_TARGET;
}

