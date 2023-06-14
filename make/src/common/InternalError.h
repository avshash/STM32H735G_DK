#ifndef _INTERNAL_ERROR_H_
#define _INTERNAL_ERROR_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    InternalError
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InternalError
{
public:
  InternalError (const char * file_name, int line_number);
};

#define INTERNAL_ASSERT(verify_clause)                      \
  if (!(verify_clause))                                     \
    {                                                       \
      throw InternalError (__FILE__, __LINE__);             \
    }

#endif // _INTERNAL_ERROR_H_
