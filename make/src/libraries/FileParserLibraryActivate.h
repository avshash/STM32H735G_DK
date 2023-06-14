#ifndef _FILE_PARSER_LIBRARY_ACTIVATE_H_
#define _FILE_PARSER_LIBRARY_ACTIVATE_H_

#include "FileParserLibrary.h"

class Library;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserLibraryActivate
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserLibraryActivate : protected FileParserLibrary
{
public:
  FileParserLibraryActivate (Libraries &, Library & library);
  ~FileParserLibraryActivate ();

protected:
  virtual void registerModulePath (const String & module_path) final;
  virtual void registerActiveLibrary (const String & library_name) final;
  virtual void registerGlobalParam (const String & param_name, const String & param_value);
};

#endif  // _FILE_PARSER_LIBRARY_ACTIVATE_H_
