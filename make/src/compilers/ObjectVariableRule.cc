#include "common.h"
#include "ObjectVariableRule.h"

#include "ActiveObject.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ObjectVariableRule
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ObjectVariableRule::ObjectVariableRule (const String & variable_name, const String & prefix, const String & postfix) :
  m_variable_name (variable_name),
  m_prefix (prefix),
  m_postfix (postfix)
{
  VERIFY_INPUT_VALID (m_variable_name[0] == '_', "Variable must begin with a '_' character.\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ObjectVariableRule
// @method:   expandVariable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ObjectVariableRule::expandVariable (ActiveObject & active_object) const
{
  String variable_value;

  if (m_prefix[0] == '_')
  {
    variable_value << active_object.getVariableValue (m_prefix);
  }
  else
  {
    variable_value << m_prefix;
  }

  if (m_postfix[0] == '_')
  {
    variable_value << active_object.getVariableValue (m_postfix);
  }
  else
  {
    variable_value << m_postfix;
  }

  active_object.appendVariable (m_variable_name.cStr (), variable_value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ObjectVariableRule
// @method:   objectType
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeObject
ObjectVariableRule::objectType () const
{
  return OBJECT_OBJECT_VARIABLE_RULE;
}




