#include "common.h"
#include "SinglePathData.h"

#include "FileParserModuleActivate.h"
#include "ActiveObject.h"
#include "Library.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SinglePathData
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SinglePathData::SinglePathData (const Library & library, const String & module_path) :
  m_library (library),
  m_path (module_path),
  m_active_objects ()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SinglePathData
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SinglePathData::~SinglePathData ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SinglePathData
// @method:   activateSingle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
SinglePathData::activateSingle (Configuration & configuration, Module & module_group)
{
  FileParserModuleActivate activate_target (configuration, module_group, m_path, *this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SinglePathData
// @method:   addActiveObject
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
SinglePathData::addActiveObject (const Compiler & compiler, Module & module_group, const String & base_name)
{
  ActiveObject * new_source = new ActiveObject (compiler);
  // Enqueue 'new_source' before exapnsion to prevent memory leaks in case of exceptions.
  m_active_objects.enqueue (new_source);

  new_source->appendVariable ("_REL_PATH", m_path);
  new_source->appendVariable ("_BASE_NAME", base_name);
  new_source->appendVariable ("_TARGET_BASE_PATH", m_library.getObjectTargetPrefix ());

  new_source->expandVariables (module_group);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SinglePathData
// @method:   writeCompilationRules
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
SinglePathData::writeCompilationRules (FileWriter & makefile) const
{
  QueueString params_list;
  m_library.appendSourceParams (params_list);

  uint32_t objects_count = m_active_objects.queueSize ();
  for (uint32_t index = 0; index < objects_count; index++)
  {
    const Object & active_object_object = m_active_objects.getMember (index);
    const ActiveObject & active_object = static_cast <const ActiveObject &> (active_object_object);

    active_object.writeMakeRule (makefile, params_list);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SinglePathData
// @method:   addObjectVariable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
SinglePathData::addObjectVariable (const char * variable_name, const String & variable_value)
{
  uint32_t objects_count = m_active_objects.queueSize ();
  for (uint32_t index = 0; index < objects_count; index++)
  {
    Object & active_object_object = m_active_objects.getMember (index);
    ActiveObject & active_object = static_cast <ActiveObject &> (active_object_object);

    active_object.appendVariable (variable_name, variable_value);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SinglePathData
// @method:   objectType
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeObject
SinglePathData::objectType () const
{
  return OBJECT_SINGLE_PATH_DATA;
}


