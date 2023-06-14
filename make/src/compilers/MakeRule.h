#ifndef _MAKE_RULE_H_
#define _MAKE_RULE_H_

#include "Object.h"

class FileWriter;
class ActiveObject;
class QueueString;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MakeRule
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MakeRule : public Object
{
public:
  virtual void writeRule (FileWriter &, const ActiveObject &, const QueueString &) const = 0;

  virtual TypeObject objectType () const;
};

#endif  // _MAKE_RULE_H_
