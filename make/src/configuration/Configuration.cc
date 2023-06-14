#include "common.h"
#include "Configuration.h"

#include "String.h"
#include "FileParserConfigurationRead.h"
#include "FileParserConfigurationTarget.h"
#include "Target.h"
#include "FileWriter.h"
#include "Central.h"
#include "Module.h"
#include "Cleanup.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Configuration::Configuration (const String & root_file_name)
: m_objects (),
  m_libraries (*this)
{
  readConfiguration (root_file_name);
  readTargets (root_file_name);
  activateTargets ();
  writeMainMakefile ();
  buildCleanup ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Configuration::~Configuration ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   registerActiveLibrary
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::registerActiveLibrary (const String & library_name)
{
  m_libraries.activateLibrary (library_name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   testObjectKey
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
Configuration::testObjectKey (const String & key) const
{
  return m_objects.testKey (key);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   getObject
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Object &
Configuration::getObject (const String & key)
{
  return m_objects.getObject (key);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   getObject
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Object &
Configuration::getObject (const String & key) const
{
  return m_objects.getObject (key);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   addObject
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::addObject (const String & key, ObjectAPtr object)
{
  return m_objects.insertObject (key, object);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   readConfiguration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::readConfiguration (const String & root_file_name)
{
  FileParserConfigurationRead configuation_reader (*this, root_file_name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   readTargets
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::readTargets (const String & root_file_name)
{
  FileParserConfigurationTarget configuation_reader (*this, root_file_name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   activateTargets
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::activateTargets ()
{
  uint32_t objects_count = m_objects.objectsCount ();
  for (uint32_t index = 0; index < objects_count; index++)
  {
    Object & cur_object = m_objects.getObject (index);
    if (cur_object.objectType () == OBJECT_TARGET)
    {
      Target & cur_target = static_cast <Target &> (cur_object);
      cur_target.activateTarget (*this, m_objects.getKey (index));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   writeMainMakefile
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::writeMainMakefile ()
{
  String main_makefile_name ("Makefile");
  FileWriter main_makefile (main_makefile_name);

  writeMakefileHeader (main_makefile);
  writeGeneralRules (main_makefile);
  writeRules (main_makefile);
  writeCompilationRules (main_makefile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   writeMakefileHeader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::writeMakefileHeader (FileWriter & makefile) const
{
  makefile << "BIN2SRC = " << Central::path (PATH_BIN_2_SOURCE) << "\n";
  makefile << "SRC2BIN = " << Central::path (PATH_SOURCE_2_BIN) << "\n";
  makefile << "\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   writeBlockHeader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::writeBlockHeader (FileWriter & makefile, const char * remark) const
{
  INTERNAL_ASSERT (remark != NULL);

  const uint32_t remark_len = strlen (remark);
  const uint32_t prefix_len = (118 - remark_len) / 2;
  const uint32_t postfix_len = 118 - remark_len - prefix_len;

  for (uint32_t index = 0; index < prefix_len; index++)
  {
    makefile << "#";
  }

  makefile << " " << remark << " ";

  for (uint32_t index = 0; index < postfix_len; index++)
  {
    makefile << "#";
  }

  makefile << "\n\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   writeGeneralRules
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::writeGeneralRules (FileWriter & makefile) const
{
  uint32_t objects_count = m_objects.objectsCount ();

  writeBlockHeader (makefile, "GENERAL RULES");
  makefile << "all: ";
  for (uint32_t index = 0; index < objects_count; index++)
  {
    const Object & cur_object = m_objects.getObject (index);
    if (cur_object.objectType () == OBJECT_TARGET)
    {
      makefile << " " << m_objects.getKey (index);
    }
  }
  makefile << "\n\n";

  makefile << "clean:\n";
  makefile << "\t./clean.sh\n\n";

  makefile << "remove:\n";
  makefile << "\t./remove_all.sh\n\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   writeRules
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::writeRules (FileWriter & makefile) const
{
  uint32_t objects_count = m_objects.objectsCount ();

  writeBlockHeader (makefile, "TARGET RULES");
  makefile << "# Target list:";
  for (uint32_t index = 0; index < objects_count; index++)
  {
    const Object & cur_object = m_objects.getObject (index);
    if (cur_object.objectType () == OBJECT_TARGET)
    {
      makefile << " '" << m_objects.getKey (index) << "'";
    }
  }
  makefile << "\n\n";

  writeTargetRules (makefile);

  writeBlockHeader (makefile, "MODULE RULES");
  makefile << "# Modules list: ";
  for (uint32_t index = 0; index < objects_count; index++)
  {
    const Object & cur_object = m_objects.getObject (index);
    if (cur_object.objectType () == OBJECT_MODULE)
    {
      const Module & cur_module = static_cast <const Module &> (cur_object);
      if (cur_module.isActive ())
      {
        makefile << " '" << m_objects.getKey (index) << "'";
      }
    }
  }
  makefile << "\n\n";

  writeModuleRules (makefile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   writeTargetRules
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::writeTargetRules (FileWriter & makefile) const
{
  uint32_t objects_count = m_objects.objectsCount ();
  for (uint32_t index = 0; index < objects_count; index++)
  {
    const Object & cur_object = m_objects.getObject (index);
    if (cur_object.objectType () == OBJECT_TARGET)
    {
      const Target & cur_target = static_cast <const Target &> (cur_object);

      cur_target.writeTargetRule (makefile);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   writeModuleRules
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::writeModuleRules (FileWriter & makefile) const
{
  uint32_t objects_count = m_objects.objectsCount ();
  for (uint32_t index = 0; index < objects_count; index++)
  {
    const Object & cur_object = m_objects.getObject (index);
    if (cur_object.objectType () == OBJECT_MODULE)
    {
      const Module & cur_module = static_cast <const Module &> (cur_object);

      cur_module.writeModuleRule (makefile);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   writeCompilationRules
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::writeCompilationRules (FileWriter & makefile) const
{
  uint32_t objects_count = m_objects.objectsCount ();

  writeBlockHeader (makefile, "OBJECTS COMPILATION RULES");

  for (uint32_t index = 0; index < objects_count; index++)
  {
    const Object & cur_object = m_objects.getObject (index);
    if (cur_object.objectType () == OBJECT_MODULE)
    {
      const Module & cur_module = static_cast <const Module &> (cur_object);
      cur_module.writeCompilationRules (makefile);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
// @method:   buildCleanup
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Configuration::buildCleanup () const
{
  Cleanup build_cleanup (m_objects);
}


