#ifndef _MAKE_RULE_CONST_H_
#define _MAKE_RULE_CONST_H_

#include "MakeRule.h"
#include "String.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MakeRuleConst
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MakeRuleConst : public MakeRule
{
public:
  MakeRuleConst (const String & rule);

  virtual void writeRule (FileWriter &, const ActiveObject &, const QueueString &) const final;

private:
  const String m_rule;
};

#endif  // _MAKE_RULE_CONST_H_
