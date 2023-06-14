#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "String.h"
#include "QueueObject.h"
#include "QueueString.h"

class FileWriter;
class ActiveObject;
class Module;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Compiler
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Compiler : public Object
{
public:
  Compiler ();
  ~Compiler ();

  void addVariableRule (const String & variable_name, const String & prefix, const String & postfix);
  void addParamRule (const String & param_name, const String & variable_name);
  void appendMakeRuleConst (const String &);
  void appendMakeRuleParam (const String & prefix, const String & variable_name, const String & postfix);

  void writeMakeRule (FileWriter &, const ActiveObject &, const QueueString &) const;
  void expandRules (ActiveObject &, Module &) const;

  virtual TypeObject objectType () const final;

private:
  void expandObjectVariables (ActiveObject &) const;
  void registerModuleParams (const ActiveObject &, Module &) const;

  QueueObject m_object_variable_rules;
  QueueObject m_module_param_rules;
  QueueObject m_make_rules;
};

#endif  // _COMPILER_H_
