#ifndef _FILE_PARSER_LIBRARY_READ_H_
#define _FILE_PARSER_LIBRARY_READ_H_

#include "FileParserLibrary.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserLibraryRead
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserLibraryRead : protected FileParserLibrary
{
public:
  FileParserLibraryRead (Libraries &, Library & library);
  ~FileParserLibraryRead ();

protected:
  virtual void registerLibrary (const String & library_name) final;
  virtual void registerLibraryParam (const String & param_name, const String & pram_val) final;
};

#endif  // _FILE_PARSER_LIBRARY_READ_H_
