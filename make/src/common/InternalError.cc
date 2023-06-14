#include "common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    InternalError
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InternalError::InternalError (const char * file_name, int line_number)
{
  printError ("Internal error (in file '%s', line %d).\n", file_name, line_number);
}

