#ifndef _INPUT_ERROR_H_
#define _INPUT_ERROR_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    InputError
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InputError
{
public:
  InputError (const char * error_message);
};

#define VERIFY_INPUT_VALID(verify_clause, error_message)    \
  if (!(verify_clause))                                     \
    {                                                       \
      throw InputError ((error_message));                   \
    }

#endif // _INPUT_ERROR_H_
