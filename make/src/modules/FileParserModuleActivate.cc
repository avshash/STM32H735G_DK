#include "common.h"
#include "FileParserModuleActivate.h"

#include "Module.h"
#include "String.h"
#include "Compiler.h"
#include "Module.h"
#include "SinglePathData.h"
#include "Configuration.h"
#include "Central.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModuleActivate
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileParserModuleActivate::FileParserModuleActivate (Configuration & configuration, Module & module_group,
                                                    const String & path, SinglePathData & single) :
  FileParserModule (configuration, path),
  m_single (single),
  m_module (module_group)
{
  m_module.appendModuleParam ("_PATH_ACTIVE", path);

  parseFile ();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModuleActivate
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileParserModuleActivate::~FileParserModuleActivate ()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModuleActivate
// @method:   registerModuleParam
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModuleActivate::registerModuleParam (const String & param_name, const String & param_value)
{
  m_module.appendModuleParam (param_name.cStr (), param_value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModuleActivate
// @method:   registerFile
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParserModuleActivate::registerFile (const String & compiler_name, const String & base_name)
{
  Configuration & configuration = getConfiguration ();

  VERIFY_INPUT_VALID (configuration.testObjectKey (compiler_name), "Compiler not defined.");
  const Object & compiler_object = configuration.getObject (compiler_name);
  VERIFY_INPUT_VALID ((compiler_object.objectType () == OBJECT_COMPILER), "Object is not a compiler.");
  const Compiler & compiler = static_cast <const Compiler &> (compiler_object);

  m_single.addActiveObject (compiler, m_module, base_name);
}


