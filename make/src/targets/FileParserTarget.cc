#include "common.h"
#include "FileParserTarget.h"

#include "Configuration.h"
#include "Library.h"
#include "Target.h"
#include "Module.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileParserTarget::FileParserTarget (Configuration & configuration, const String & input_file) :
  FileParser (input_file),
  m_configuration (configuration)
{
  parseFile ();
  generateTarget ();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileParserTarget::~FileParserTarget ()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
// @method:   registerDirective
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserTarget::registerDirective (const String & directive)
{
  if (directive == "TARGET_NAME")
  {
    return registerTargetName ();
  }

  if (directive == "LIBRARY_NAME")
  {
    return registerLibraryName ();
  }

  if (directive == "TARGET_PARAM")
  {
    return registerTargetParam ();
  }

  fprintf (stderr, "Directive is '%s'.\n", directive.cStr ());
  VERIFY_INPUT_VALID (false, "Unknown target directive.");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
// @method:   registerTargetName
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserTarget::registerTargetName ()
{
  String target_name = readNextString (true);
  VERIFY_INPUT_VALID (m_target_name == NULL, "Multiple target names defined.");

  m_target_name << target_name;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
// @method:   registerLibraryName
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserTarget::registerLibraryName ()
{
  String library_name = readNextString (false);

  VERIFY_INPUT_VALID (m_library_name == NULL, "Multiple parent library defined.");
  VERIFY_INPUT_VALID (m_configuration.testObjectKey (library_name), "Library not defined.");

  const Object & library_object = m_configuration.getObject (library_name);
  VERIFY_INPUT_VALID (library_object.objectType () == OBJECT_LIBRARY, "Object is not a library.");

  m_library_name << library_name;
  m_configuration.registerActiveLibrary (library_name);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
// @method:   registerTargetParam
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserTarget::registerTargetParam ()
{
  String variable_name = readNextString (false);
  String variable_value = readNextString (true);

  m_target_params.enqueue (variable_name);
  m_target_params.enqueue (variable_value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
// @method:   generateTarget
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserTarget::generateTarget ()
{
  VERIFY_INPUT_VALID (m_target_name != NULL, "Missing target name definition.");
  VERIFY_INPUT_VALID (m_library_name != NULL, "Missing parent library definitions.");

  Object & library_object = m_configuration.getObject (m_library_name);
  Library & library = static_cast <Library &> (library_object);

  appendDefaultParams (library);

  Target * new_target = new Target (library, m_target_params);
  m_configuration.addObject (m_target_name, new_target);

  generateActiveObject (*new_target);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
// @method:   appendDefaultParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserTarget::appendDefaultParams (const Library & library)
{
  String target_module_name;
  target_module_name << "_TARGET_MODULE_" << m_target_name;

  m_target_params.enqueue ("MODULE_DEPEND");
  m_target_params.enqueue (library.getDefaultModuleName ());

  m_target_params.enqueue ("MODULE_DEPEND");
  m_target_params.enqueue (target_module_name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
// @method:   generateActiveObject
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserTarget::generateActiveObject (Target & target)
{
  String target_module_name;
  target_module_name << "_TARGET_MODULE_" << m_target_name;

  VERIFY_INPUT_VALID (!m_configuration.testObjectKey (target_module_name), "Module target name already defined.");

  Module * module_ptr = new Module ();
  m_configuration.addObject (target_module_name, module_ptr);

  Library & target_library = target.targetLibrary ();
  module_ptr->addSinglePathData (target_library, target_library.getPath ());

  module_ptr->appendModuleParam ("_BASE_TARGET_NAME", m_target_name);
}

