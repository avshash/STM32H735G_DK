#ifndef _ARRAY_H_
#define _ARRAY_H_

class Object;
class Action;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Array
{
public:
  void DoAction (Action & action);
  void DoAction (const Action & action) const;

protected:
  Array ();
  ~Array ();

  void insertObject (uint32_t index, Object *);
  Object * removeObject (uint32_t index);
  void expandArray ();

  Object * getObject (uint32_t index);
  const Object * getObject (uint32_t index) const;
  uint32_t getObjectsCount () const;
  uint32_t getObjectsSize () const;

  virtual void replaceArray (Object ** old_array, uint32_t old_size) noexcept = 0;

private:
  uint32_t m_objects_count;
  uint32_t m_objects_size;
  Object ** m_objects;
};

#endif  // _ARRAY_H_
