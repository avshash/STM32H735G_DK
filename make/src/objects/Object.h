#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "TypeObject.h"

class String;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Object
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Object
{
public:
  virtual ~Object ();

  virtual TypeObject objectType () const = 0;

protected:
  Object ();
};


#endif  // _OBJECT_H_
