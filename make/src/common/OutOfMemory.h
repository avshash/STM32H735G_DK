#ifndef _OUT_OF_MEMORY_H_
#define _OUT_OF_MEMORY_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    OutOfMemory
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OutOfMemory
{
public:
  OutOfMemory (const char * file_name, int line_number);
};

#define TEST_MALLOC_OUT_OF_MEMORY(pointer)                  \
  if ((pointer) == NULL)                                    \
  {                                                         \
    throw OutOfMemory (__FILE__, __LINE__);                 \
  }

#endif // _OUT_OF_MEMORY_H_
