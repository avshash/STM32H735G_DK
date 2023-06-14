#include "common.h"
#include "SetStringObject.h"

#include "StringObjectPair.h"

#define MAP_SIZE ((2 * m_objects_size) + 1)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SetStringObject::SetStringObject ()
: m_objects_count (0),
  m_objects_size (0),
  m_objects_map (NULL),
  m_objects (NULL)
{
  expandArray ();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SetStringObject::~SetStringObject ()
{
  if (m_objects != NULL)
  {
    for (uint32_t index = 0; index < m_objects_count; index++)
    {
      StringObjectPair * next_pair = m_objects[index];
      if (next_pair != NULL)
      {
        delete next_pair;
      }
    }

    free (m_objects);
  }

  if (m_objects_map != NULL)
  {
    free (m_objects_map);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   findObjectIndex
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
SetStringObject::findObjectIndex (const String & target_key) const
{
  INTERNAL_ASSERT (m_objects_count < MAP_SIZE);

  uint32_t index = target_key.hash (m_objects_size);
  while (1)
  {
    uint32_t object_index = m_objects_map[index];
    if (object_index == 0xFFFFFFFFUL)
    {
      break;
    }

    StringObjectPair * next_pair = m_objects[object_index];
    if (target_key == next_pair->getKey ())
    {
      break;
    }

    index++;
    if (MAP_SIZE <= index)
    {
      index = 0;
    }
  }

  return index;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   testKey
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
SetStringObject::testKey (const String & key) const
{
  uint32_t index = findObjectIndex (key);

  return (m_objects_map[index] != 0xFFFFFFFFUL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   objectsCount
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
SetStringObject::objectsCount () const
{
  return m_objects_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   getObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Object &
SetStringObject::getObject (uint32_t object_index)
{
  INTERNAL_ASSERT (object_index < m_objects_count);

  return m_objects[object_index]->getObject ();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   getObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Object &
SetStringObject::getObject (const String & key)
{
  uint32_t index = findObjectIndex (key);
  uint32_t object_index = m_objects_map[index];

  return getObject (object_index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   getObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Object &
SetStringObject::getObject (uint32_t object_index) const
{
  INTERNAL_ASSERT (object_index < m_objects_count);

  return m_objects[object_index]->getObject ();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   getObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Object &
SetStringObject::getObject (const String & key) const
{
  uint32_t index = findObjectIndex (key);
  uint32_t object_index = m_objects_map[index];

  return getObject (object_index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   getKey
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const String &
SetStringObject::getKey (uint32_t object_index) const
{
  INTERNAL_ASSERT (object_index < m_objects_count);

  return m_objects[object_index]->getKey ();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   insertObject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
SetStringObject::insertObject (const String & key, ObjectAPtr new_object)
{
  if (m_objects_size <= m_objects_count)
  {
    expandArray ();
  }

  uint32_t index = findObjectIndex (key);
  INTERNAL_ASSERT (m_objects_map[index] == 0xFFFFFFFFUL);

  m_objects[m_objects_count] = new StringObjectPair (key, new_object);
  m_objects_map[index] = m_objects_count;
  m_objects_count++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   rehash
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
SetStringObject::rehash ()
{
  if (m_objects_map != NULL)
  {
    free (m_objects_map);
  }

  m_objects_map = (uint32_t *) malloc (MAP_SIZE * sizeof (uint32_t));
  TEST_MALLOC_OUT_OF_MEMORY (m_objects_map);
  memset (m_objects_map, 0xFFU, MAP_SIZE * sizeof (uint32_t));

  for (uint32_t index = 0; index < m_objects_count; index++)
  {
    StringObjectPair * cur_object = m_objects[index];
    INTERNAL_ASSERT (cur_object != NULL);

    uint32_t map_index = findObjectIndex (cur_object->getKey ());
    INTERNAL_ASSERT (m_objects_map[map_index] == 0xFFFFFFFFUL);

    m_objects_map[map_index] = index;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SetStringObject
// @method:   expandArray
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
SetStringObject::expandArray ()
{
  uint32_t new_size = (2 * m_objects_size) + 1;
  StringObjectPair ** new_array = (StringObjectPair **) malloc (new_size * sizeof (StringObjectPair *));
  TEST_MALLOC_OUT_OF_MEMORY (new_array);

  memset (new_array, 0, new_size * sizeof (StringObjectPair *));

  if (m_objects != NULL)
  {
    memcpy (new_array, m_objects, m_objects_count * sizeof (StringObjectPair *));
    free (m_objects);
  }

  m_objects = new_array;
  m_objects_size = new_size;
  rehash ();
}

