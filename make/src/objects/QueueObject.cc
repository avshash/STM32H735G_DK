#include "common.h"
#include "QueueObject.h"

#include "Object.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    QueueObject
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QueueObject::QueueObject ()
: m_objects_count (0),
  m_objects_size (2),
  m_objects ((Object **) malloc (m_objects_size * sizeof (Object *)))
{
  TEST_MALLOC_OUT_OF_MEMORY (m_objects);
  memset (m_objects, 0, m_objects_size * sizeof (Object *));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    QueueObject
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
QueueObject::~QueueObject ()
{
  if (m_objects != NULL)
  {
    for (uint32_t index = 0; index < m_objects_count; index++)
    {
      Object * next_object = m_objects[index];
      if (next_object != NULL)
      {
        delete next_object;
      }
    }

    free (m_objects);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    QueueObject
// @method:   enqueue
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
QueueObject::enqueue (ObjectAPtr new_object)
{
  verifySpace ();

  m_objects[m_objects_count] = new_object;
  m_objects_count++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    QueueObject
// @method:   queueSize
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
QueueObject::queueSize () const
{
  return m_objects_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    QueueObject
// @method:   getMember
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Object &
QueueObject::getMember (uint32_t index)
{
  INTERNAL_ASSERT (index < m_objects_count);

  Object * member = m_objects[index];
  INTERNAL_ASSERT (member != NULL);

  return *member;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    QueueObject
// @method:   getMember
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Object &
QueueObject::getMember (uint32_t index) const
{
  INTERNAL_ASSERT (index < m_objects_count);

  const Object * member = m_objects[index];
  INTERNAL_ASSERT (member != NULL);

  return *member;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    QueueObject
// @method:   verifySpace
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
QueueObject::verifySpace ()
{
  if (m_objects_count < m_objects_size)
  {
    return;
  }

  Object ** new_objects = ((Object **) malloc (2 * m_objects_size * sizeof (Object *)));
  TEST_MALLOC_OUT_OF_MEMORY (new_objects);
  memset (new_objects, 0, 2 * m_objects_size * sizeof (Object *));
  memcpy (new_objects, m_objects, m_objects_size * sizeof (Object *));

  free (m_objects);
  m_objects = new_objects;
  m_objects_size = 2 * m_objects_size;
}

