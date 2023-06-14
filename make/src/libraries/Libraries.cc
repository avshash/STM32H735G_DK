#include "common.h"
#include "Libraries.h"

#include "FileParserLibraryRead.h"
#include "FileParserLibraryActivate.h"
#include "Library.h"
#include "Configuration.h"
#include "FileParserModuleRead.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Libraries
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Libraries::Libraries (Configuration & configuration)
: m_configuration (configuration)
{
  const String root_name ("_ROOT_LIBRARY");
  const String root_path ("");

  registerLibrary (root_name, new Library (root_path));
  activateLibrary (root_name);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Libraries
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Libraries::~Libraries ()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Libraries
// @method:   registerLibrary
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Libraries::registerLibrary (const String & library_name, ObjectAPtr new_library)
{
  VERIFY_INPUT_VALID (!m_configuration.testObjectKey (library_name), "Library name must be unique.");
  m_configuration.addObject (library_name, new_library);

  Object & library_object = m_configuration.getObject (library_name);
  INTERNAL_ASSERT (library_object.objectType () == OBJECT_LIBRARY);
  Library & library = static_cast <Library &> (library_object);

  FileParserLibraryRead tree_reader (*this, library);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Libraries
// @method:   activateLibrary
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Libraries::activateLibrary (const String & library_key)
{
  fprintf (stdout, "Activate library '%s'.\n", library_key.cStr ());
  if (!m_configuration.testObjectKey (library_key))
  {
    VERIFY_INPUT_VALID (false, "Active library not defined.");
  }

  Object & active_library_object = m_configuration.getObject (library_key);
  VERIFY_INPUT_VALID (active_library_object.objectType () == OBJECT_LIBRARY, "Object activated is not a library.");
  Library & active_library = static_cast <Library &> (active_library_object);

  FileParserLibraryActivate library_activate (*this, active_library);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Libraries
// @method:   registerModule
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Libraries::registerModule (const Library & library, const String & module_path)
{
  String complete_path (library.getPath ());
  complete_path << module_path << "/";

  FileParserModuleRead module_reader (m_configuration, library, complete_path);
}


