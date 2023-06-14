#ifndef _MAKE_RULE_PARAM_H_
#define _MAKE_RULE_PARAM_H_

#include "MakeRule.h"
#include "String.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MakeRuleParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MakeRuleParam : public MakeRule
{
public:
  MakeRuleParam (const String & prefix, const String & param_name, const String & postfix);

  virtual void writeRule (FileWriter &, const ActiveObject &, const QueueString &) const final;

private:
  void writeRuleExpanded (FileWriter &, const String & prefix, const String & postfix, const QueueString &) const;

  const String m_prefix;
  const String m_param_name;
  const String m_postfix;
};

#endif  // _MAKE_RULE_PARAM_H_
