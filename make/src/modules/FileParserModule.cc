#include "common.h"
#include "FileParserModule.h"

#include "String.h"
#include "Configuration.h"
#include "Module.h"
#include "Compiler.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileParserModule::FileParserModule (Configuration & configuration, const String & path)
: FileParser ("files.make", path),
  m_configuration (configuration)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileParserModule::~FileParserModule ()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   registerDirective
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModule::registerDirective (const String & directive)
{
  if (directive == "MODULE_NAME")
  {
    return registerModuleName ();
  }

  if (directive == "DIR_MODULE")
  {
    return registerModule ();
  }

  if (directive == "MODULE_PARAM")
  {
    return registerModuleParam ();
  }

  if (directive == "COMPILE_FILE")
  {
    return registerFile ();
  }

  VERIFY_INPUT_VALID (false, "Unknown module directive.");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   registerModuleName
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModule::registerModuleName ()
{
  String module_name = readNextString (false);
  registerModuleName (module_name);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   registerModule
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModule::registerModule ()
{
  String module_path = readNextString (false);
  registerModule (module_path);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   registerModuleParam
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModule::registerModuleParam ()
{
  String param_name = readNextString (false);
  String param_value = readNextString (true);
  registerModuleParam (param_name, param_value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   registerFile
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModule::registerFile ()
{
  String compiler_name = readNextString (false);
  String base_name = readNextString (false);

  registerFile (compiler_name, base_name);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   getModule
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Module &
FileParserModule::getModule (const String & module_name, bool b_create_new_module)
{
  if (!m_configuration.testObjectKey (module_name))
  {
    VERIFY_INPUT_VALID (b_create_new_module, "Module name missing.");

    fprintf (stdout, "Add module '%s'.\n", module_name.cStr ());
    Module * module_ptr = new Module ();
    m_configuration.addObject (module_name, module_ptr);
  }

  Object & module_object = m_configuration.getObject (module_name);
  if (module_object.objectType () != OBJECT_MODULE)
  {
    VERIFY_INPUT_VALID (b_create_new_module, "Module name missing.");
    VERIFY_INPUT_VALID (false, "Module name must be unique.");
  }

  Module & cur_module = static_cast <Module &> (module_object);
  return cur_module;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   getCompiler
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Compiler &
FileParserModule::getCompiler (const String & compiler_name) const
{
  VERIFY_INPUT_VALID (m_configuration.testObjectKey (compiler_name), "Compiler not defined.");

  Object & compiler_object = m_configuration.getObject (compiler_name);
  VERIFY_INPUT_VALID ((compiler_object.objectType () == OBJECT_COMPILER), "Object is not a compiler.");

  const Compiler & compiler = static_cast <const Compiler &> (compiler_object);
  return compiler;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   getConfiguration
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Configuration &
FileParserModule::getConfiguration ()
{
  return m_configuration;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   registerModuleName
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModule::registerModuleName (const String & module_name)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   registerModule
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModule::registerModule (const String & sub_module)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   registerModuleParam
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModule::registerModuleParam (const String &, const String &)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   registerFile
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModule::registerFile (const String &, const String &)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
// @method:   defaultModuleName
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *
FileParserModule::defaultModuleName ()
{
  return "_MODULE_ROOT";
}

