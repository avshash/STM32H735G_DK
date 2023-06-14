#include "common.h"
#include "Cleanup.h"

#include "String.h"
#include "Target.h"
#include "Module.h"
#include "FileWriter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Cleanup
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Cleanup::Cleanup (const SetStringObject & global_objects) :
  m_clean_files (),
  m_empty_files (),
  m_paths ()
{
  parseObjects (global_objects);

  buildClean ();
  buildRemoveAll ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Cleanup
// @method:   destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Cleanup::~Cleanup ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Cleanup
// @method:   parseObjects
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Cleanup::parseObjects (const SetStringObject & global_objects)
{
  uint32_t objects_count = global_objects.objectsCount ();
  for (uint32_t index = 0; index < objects_count; index++)
  {
    const Object & cur_object = global_objects.getObject (index);

    if (cur_object.objectType () == OBJECT_TARGET)
    {
      const Target & cur_target = static_cast <const Target &> (cur_object);
      parseParamsList (cur_target.getTargetParams ());
    }

    if (cur_object.objectType () == OBJECT_MODULE)
    {
      const Module & cur_module = static_cast <const Module &> (cur_object);
      parseParamsList (cur_module.getModuleParams ());
    }
  }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Cleanup
// @method:   parseParamsList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Cleanup::parseParamsList (const QueueString & params_list)
{
  uint32_t params_count = params_list.queueSize ();
  for (uint32_t variable_index = 0; variable_index < params_count; variable_index = variable_index + 2)
  {
    const String & variable_name = params_list.getMember (variable_index);

    if (variable_name == "TARGET_CLEAN_FILE")
    {
      const String & clean_file_name = params_list.getMember (variable_index + 1);

      m_clean_files.enqueue (clean_file_name);
      registerPath (clean_file_name);
    }

    if (variable_name == "TARGET_EMPTY_FILE")
    {
      const String & empty_file_name = params_list.getMember (variable_index + 1);

      m_empty_files.enqueue (empty_file_name);
      registerPath (empty_file_name);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Cleanup
// @method:   registerPath
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Cleanup::registerPath (const String & file_name)
{
  const char * c_file_name = file_name.cStr ();

  for (uint32_t offset = 0; c_file_name[offset] != 0; offset++)
  {
    if (c_file_name[offset] == '/')
    {
      String cur_path (c_file_name, offset);
      if (!m_paths.testKey (cur_path))
      {
        m_paths.insertObject (cur_path, NULL);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Cleanup
// @method:   buildClean
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Cleanup::buildClean () const
{
  String clean_file_name ("clean.sh");
  FileWriter clean_file (clean_file_name);

  clean_file << "#!/bin/bash\n";
  clean_file << "set -e\n";
  clean_file << "set -o pipefail\n";
  clean_file << "\n";
  clean_file << "# Clean build.\n";
  clean_file << "\n";

  clean_file << "# Make paths.\n";
  uint32_t paths_count = m_paths.objectsCount ();
  for (uint32_t index = 0; index < paths_count; index++)
  {
    const String & cur_path = m_paths.getKey (index);
    clean_file << "mkdir -p " << cur_path << "\n";
  }
  clean_file << "\n";

  clean_file << "# remove clean files.\n";
  uint32_t clean_files_count = m_clean_files.queueSize ();
  for (uint32_t index = 0; index < clean_files_count; index++)
  {
    const String & target_file = m_clean_files.getMember (index);
    clean_file << "rm -f " << target_file << "\n";
  }
  clean_file << "\n";

  clean_file << "# Create empty files.\n";
  uint32_t empty_files_count = m_empty_files.queueSize ();
  for (uint32_t index = 0; index < empty_files_count; index++)
  {
    const String & empty_file = m_empty_files.getMember (index);
    clean_file << "echo \"\" > " << empty_file << "\n";
  }
  clean_file << "\n";

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Cleanup
// @method:   buildRemoveAll
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Cleanup::buildRemoveAll () const
{
  String remove_all_file_name ("remove_all.sh");
  FileWriter remove_all_file (remove_all_file_name);

  remove_all_file << "#!/bin/bash\n";
  remove_all_file << "set -e\n";
  remove_all_file << "set -o pipefail\n";
  remove_all_file << "\n";
  remove_all_file << "# Remove all files (cleanup).\n";
  remove_all_file << "\n";

  remove_all_file << "# remove clean files.\n";
  uint32_t clean_files_count = m_clean_files.queueSize ();
  for (uint32_t index = 0; index < clean_files_count; index++)
  {
    const String & target_file = m_clean_files.getMember (index);
    remove_all_file << "rm -f " << target_file << "\n";
  }
  remove_all_file << "\n";

  remove_all_file << "# Remove empty files.\n";
  uint32_t empty_files_count = m_empty_files.queueSize ();
  for (uint32_t index = 0; index < empty_files_count; index++)
  {
    const String & empty_file = m_empty_files.getMember (index);
    remove_all_file << "rm -f " << empty_file << "\n";
  }
  remove_all_file << "\n";

  remove_all_file << "# Remove paths.\n";
  uint32_t paths_count = m_paths.objectsCount ();
  for (uint32_t index = 1; index <= paths_count; index++)
  {
    const String & cur_path = m_paths.getKey (paths_count - index);
    remove_all_file << "if test -e \"" << cur_path << "\"\n";
    remove_all_file << "  then\n";
    remove_all_file << "    rmdir --ignore-fail-on-non-empty " << cur_path << "\n";
    remove_all_file << "fi\n";
  }
  remove_all_file << "\n";

}

