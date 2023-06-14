#include "common.h"
#include "Compiler.h"

#include "FileWriter.h"
#include "ObjectVariableRule.h"
#include "ModuleParamRule.h"
#include "MakeRuleConst.h"
#include "MakeRuleParam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Compiler::Compiler () :
  m_object_variable_rules (),
  m_module_param_rules (),
  m_make_rules ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Compiler::~Compiler ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   addVariableRule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Compiler::addVariableRule (const String & variable_name, const String & prefix, const String & postfix)
{
  Object * variable_rule = new ObjectVariableRule (variable_name, prefix, postfix);
  m_object_variable_rules.enqueue (variable_rule);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   addParamRule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Compiler::addParamRule (const String & param_name, const String & variable_name)
{
  Object * param_rule = new ModuleParamRule (param_name, variable_name);
  m_module_param_rules.enqueue (param_rule);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   appendMakeRuleConst
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Compiler::appendMakeRuleConst (const String & param)
{
  Object * make_rule = new MakeRuleConst (param);
  m_make_rules.enqueue (make_rule);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   appendMakeRuleParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Compiler::appendMakeRuleParam (const String & prefix, const String & variable_name, const String & postfix)
{
  Object * make_rule = new MakeRuleParam (prefix, variable_name, postfix);
  m_make_rules.enqueue (make_rule);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   writeMakeRule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Compiler::writeMakeRule (FileWriter & makefile,
                         const ActiveObject & active_object, const QueueString & library_params) const
{
  uint32_t make_rules_count = m_make_rules.queueSize ();
  for (uint32_t index = 0; index < make_rules_count; index++)
  {
    const Object & cur_make_rule_object = m_make_rules.getMember (index);
    const MakeRule & cur_make_rule = static_cast <const MakeRule &> (cur_make_rule_object);
    cur_make_rule.writeRule (makefile, active_object, library_params);
  }

  makefile << "\n\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   expandRules
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Compiler::expandRules (ActiveObject & active_object, Module & module_group) const
{
  expandObjectVariables (active_object);
  registerModuleParams (active_object, module_group);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   expandObjectVariables
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Compiler::expandObjectVariables (ActiveObject & active_object) const
{
  uint32_t object_variables_count = m_object_variable_rules.queueSize ();
  for (uint32_t index = 0; index < object_variables_count; index++)
  {
    const Object & cur_variable_object = m_object_variable_rules.getMember (index);
    const ObjectVariableRule & cur_variable = static_cast <const ObjectVariableRule &> (cur_variable_object);
    cur_variable.expandVariable (active_object);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   registerModuleParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Compiler::registerModuleParams (const ActiveObject & active_object, Module & module_group) const
{
  uint32_t module_params_count = m_module_param_rules.queueSize ();
  for (uint32_t index = 0; index < module_params_count; index++)
  {
    const Object & cur_module_param_object = m_module_param_rules.getMember (index);
    const ModuleParamRule & cur_module_param = static_cast <const ModuleParamRule &> (cur_module_param_object);
    cur_module_param.registerModuleParam (active_object, module_group);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
// @method:   objectType
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeObject
Compiler::objectType () const
{
  return OBJECT_COMPILER;
}

