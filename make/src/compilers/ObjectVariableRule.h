#ifndef _OBJECT_VARIABLE_RULE_H_
#define _OBJECT_VARIABLE_RULE_H_

#include "Object.h"
#include "String.h"

class ActiveObject;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ObjectVariableRule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ObjectVariableRule : public Object
{
public:
  ObjectVariableRule (const String & variable_name, const String & prefix, const String & postfix);

  void expandVariable (ActiveObject &) const;

  virtual TypeObject objectType () const;

private:
  const String m_variable_name;
  const String m_prefix;
  const String m_postfix;

};

#endif  // _OBJECT_VARIABLE_RULE_H_
