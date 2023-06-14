#include "common.h"
#include "MakeRuleParam.h"

#include "FileWriter.h"
#include "ActiveObject.h"
#include "QueueString.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MakeRuleParam
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MakeRuleParam::MakeRuleParam (const String & prefix, const String & param_name, const String & postfix) :
  m_prefix (prefix),
  m_param_name (param_name),
  m_postfix (postfix)
{
  INTERNAL_ASSERT (m_param_name != NULL);
  VERIFY_INPUT_VALID (m_param_name[0] != '_', "Param name can not begin with a '_' character.\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MakeRuleParam
// @method:   writeRule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MakeRuleParam::writeRule (FileWriter & makefile, const ActiveObject & active_object,
                          const QueueString & params_list) const 
{
  String prefix_value;
  String postfix_value;

  if (m_prefix[0] == '_')
  {
    prefix_value << active_object.getVariableValue (m_prefix);
  }
  else
  {
    prefix_value << m_prefix;
  }

  if (m_postfix[0] == '_')
  {
    postfix_value << active_object.getVariableValue (m_postfix);
  }
  else
  {
    postfix_value << m_postfix;
  }

  writeRuleExpanded (makefile, prefix_value, postfix_value, params_list);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MakeRuleParam
// @method:   writeRuleExpanded
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
MakeRuleParam::writeRuleExpanded (FileWriter & makefile, const String & prefix, const String & postfix,
                                  const QueueString & params_list) const 
{
  uint32_t params_list_size = params_list.queueSize ();
  for (uint32_t index = 0; index < params_list_size; index = index + 2)
  {
    if (m_param_name == params_list.getMember (index))
    {
      const String & param_value = params_list.getMember (index + 1);
      makefile << prefix << param_value << postfix;
    }
  }
}

