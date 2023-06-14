#ifndef _OBJECT_A_PTR_H_
#define _OBJECT_A_PTR_H_

class Object;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ObjectAPtr
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ObjectAPtr
{
public:
  ObjectAPtr (Object *);
  ObjectAPtr (ObjectAPtr & source);
  ~ObjectAPtr ();

  operator Object * ();
  Object & operator* ();
  const Object & operator* () const;
  Object * operator-> ();
  const Object * operator-> () const;

private:
// Not implemented.
  ObjectAPtr ();
  ObjectAPtr & operator= (ObjectAPtr &);

  Object * m_object;
};


#endif  // _OBJECT_A_PTR_H_
