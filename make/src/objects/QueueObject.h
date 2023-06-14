#ifndef _QUEUE_OBJECT_H_
#define _QUEUE_OBJECT_H_

#include "ObjectAPtr.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    QueueObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class QueueObject
{
public:
  QueueObject ();
  ~QueueObject ();

  void enqueue (ObjectAPtr);
//  Object * dequeue ();

  uint32_t queueSize () const;
  Object & getMember (uint32_t index);
  const Object & getMember (uint32_t index) const;

private:
  void verifySpace ();

  uint32_t m_objects_count;
  uint32_t m_objects_size;
  Object ** m_objects;
};

#endif  // _QUEUE_OBJECT_H_
