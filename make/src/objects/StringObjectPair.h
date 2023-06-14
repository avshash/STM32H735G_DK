#ifndef _STRING_OBJECT_PAIR_H_
#define _STRING_OBJECT_PAIR_H_

#include "String.h"
#include "ObjectAPtr.h"

class Object;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    StringObjectPair
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StringObjectPair : protected String
{
public:
  StringObjectPair (const String & key, ObjectAPtr object);
  ~StringObjectPair ();

  uint32_t hash (uint32_t mod) const;

  const String & getKey () const;
  Object & getObject ();
  const Object & getObject () const;

private:
  String m_key;
  Object * m_object;
};

#endif  // _STRING_OBJECT_PAIR_H_
