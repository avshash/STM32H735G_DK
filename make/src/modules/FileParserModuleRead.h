#ifndef _FILE_PARSER_MODULE_READ_H_
#define _FILE_PARSER_MODULE_READ_H_

#include "FileParserModule.h"

class Configuration;
class Library;
class String;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModuleRead
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserModuleRead : protected FileParserModule
{
public:
  FileParserModuleRead (Configuration &, const Library &, const String & path);
  ~FileParserModuleRead ();

protected:
  virtual void registerModuleName (const String & module_name) final;
  virtual void registerModule (const String & sub_module) final;

private:
  void appendSingle ();

  const Library & m_library;
  String m_module_name;
};

#endif  // _FILE_PARSER_MODULE_READ_H_
