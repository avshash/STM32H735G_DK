#ifndef _MODULE_PARAM_RULE_H_
#define _MODULE_PARAM_RULE_H_

#include "Object.h"
#include "String.h"

class ActiveObject;
class Module;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleParamRule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleParamRule : public Object
{
public:
  ModuleParamRule (const String & param_name, const String & variable_name);

  void registerModuleParam (const ActiveObject &, Module &) const;

  virtual TypeObject objectType () const;

private:
  const String m_param_name;
  const String m_variable_name;
};

#endif  // _MODULE_PARAM_RULE_H_
