#ifndef _SET_STRING_OBJECT_H_
#define _SET_STRING_OBJECT_H_

#include "ObjectAPtr.h"

class String;
class Object;
class StringObjectPair;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SetStringObject
{
public:
  SetStringObject ();
  ~SetStringObject ();

  bool testKey (const String & key) const;
  uint32_t objectsCount () const;
  Object & getObject (uint32_t index);
  Object & getObject (const String & key);
  const Object & getObject (uint32_t index) const;
  const Object & getObject (const String & key) const;
  const String & getKey (uint32_t index) const;
  void insertObject (const String & key, ObjectAPtr);


private:
  uint32_t findObjectIndex (const String &) const;
  void rehash ();
  void expandArray ();

  uint32_t m_objects_count;
  uint32_t m_objects_size;
  uint32_t * m_objects_map;
  StringObjectPair ** m_objects;

};

#endif  // _SET_STRING_OBJECT_H_
