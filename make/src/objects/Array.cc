#include "common.h"
#include "Array.h"

#include "Object.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Array::Array ()
: m_objects_count (0),
  m_objects_size (3),
  m_objects ((Object **) malloc (sizeof (Object *) * m_objects_size))
{
  TEST_MALLOC_OUT_OF_MEMORY (m_objects);
  memset (m_objects, 0, sizeof (Object *) * m_objects_size);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Array::~Array ()
{
  if (m_objects != NULL)
  {
    for (uint32_t offset = 0; offset < m_objects_size; offset++)
    {
      if (m_objects[offset] != NULL)
      {
        delete m_objects[offset];
      }
    }

    free (m_objects);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
// @method:   expandArray
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Array::expandArray ()
{
  uint32_t new_size = (m_objects_size * 2) + 1;
  Object ** new_objects = (Object **) malloc (new_size * sizeof (Object *));
  TEST_MALLOC_OUT_OF_MEMORY (new_objects);
  memset (new_objects, 0, new_size * sizeof (Object *));

  Object ** old_array = m_objects;
  uint32_t old_size = m_objects_size;

  m_objects_count = 0;
  m_objects = new_objects;
  m_objects_size = new_size;

  replaceArray (old_array, old_size);
  free (old_array);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
// @method:   insertObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Array::insertObject (uint32_t index, Object * new_object)
{
  INTERNAL_ASSERT (index < m_objects_size);
  INTERNAL_ASSERT (m_objects[index] == NULL);

  m_objects[index] = new_object;
  m_objects_count++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
// @method:   removeObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Object *
Array::removeObject (uint32_t index)
{
  INTERNAL_ASSERT (index < m_objects_size);
  Object * ret_val = m_objects[index];
  m_objects[index] = NULL;
  m_objects_count--;

  return ret_val;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
// @method:   getObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Object *
Array::getObject (uint32_t index)
{
  INTERNAL_ASSERT (index < m_objects_size);
  return m_objects[index];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
// @method:   getObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Object *
Array::getObject (uint32_t index) const
{
  INTERNAL_ASSERT (index < m_objects_size);
  return m_objects[index];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
// @method:   getObjectsCount
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
Array::getObjectsCount () const
{
  return m_objects_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Array
// @method:   getObjectsSize
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
Array::getObjectsSize () const
{
  return m_objects_size;
}

