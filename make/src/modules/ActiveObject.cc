#include "common.h"
#include "ActiveObject.h"

#include "Compiler.h"
#include "Library.h"
#include "Central.h"
#include "Module.h"

char ActiveObject::m_global_uid[4] = {'0', '0', '0', 0};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ActiveObject::ActiveObject (const Compiler & compiler) :
  m_compiler (compiler),
  m_variables_list ()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ActiveObject::~ActiveObject ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
// @method:   expandVariables
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ActiveObject::expandVariables (Module & module_group)
{
  advanceUid ();

  appendVariable ("_FILE_UNIQUE_ID_HEX3", m_global_uid);
  appendVariable ("_BIN_2_SOURCE", Central::path (PATH_BIN_2_SOURCE));
  appendVariable ("_SOURCE_2_BIN", Central::path (PATH_SOURCE_2_BIN));

  const QueueString & module_params = module_group.getModuleParams ();
  uint32_t params_count = module_params.queueSize ();
  for (uint32_t index = 0; index < params_count; index = index + 2)
  {
    const String & param_name = module_params.getMember (index);
    if (param_name[0] == '_')
    {
      appendVariable (param_name.cStr (), module_params.getMember (index + 1));
    }
  }

  m_compiler.expandRules (*this, module_group);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
// @method:   writeMakeRule
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ActiveObject::writeMakeRule (FileWriter & makefile, const QueueString & params_list) const
{
  m_compiler.writeMakeRule (makefile, *this, params_list);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
// @method:   getVariableValue
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const String &
ActiveObject::getVariableValue (const String & variable_name) const
{
  uint32_t variables_count = m_variables_list.queueSize ();
  for (uint32_t index = 0; index < variables_count; index = index + 2)
  {
    if (variable_name == m_variables_list.getMember (index))
    {
      return m_variables_list.getMember (index + 1);
    }
  }

  VERIFY_INPUT_VALID (false, "Compiler missing definitions.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
// @method:   appendVariable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ActiveObject::appendVariable (const char * variable_name, const String & variable_val)
{
  m_variables_list.enqueue (variable_name);
  m_variables_list.enqueue (variable_val);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
// @method:   appendVariable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ActiveObject::appendVariable (const char * variable_name, const char * variable_val)
{
  m_variables_list.enqueue (variable_name);
  m_variables_list.enqueue (variable_val);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
// @method:   advanceUid
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ActiveObject::advanceUid ()
{
  switch (m_global_uid[2])
  {
    case 'F':
      m_global_uid[2] = '0';
      switch (m_global_uid[1])
      {
        case 'F':
          m_global_uid[1] = '0';
          VERIFY_INPUT_VALID ((m_global_uid[0] != 'F'), "Too many source files.");
          if (m_global_uid[0] == '9')
          {
            m_global_uid[0] = 'A';
          }
          else
          {
            m_global_uid[0]++;
          }
          break;

        case '9':
          m_global_uid[1] = 'A';
          break;

        default:
          m_global_uid[1]++;
          break;
      }
      break;

    case '9':
      m_global_uid[2] = 'A';
      break;

    default:
      m_global_uid[2]++;
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
// @method:   objectType
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeObject
ActiveObject::objectType () const
{
  return OBJECT_ACTIVE_OBJECT;
}


