#ifndef _FILE_PARSER_CONFIGURATION_READ_H_
#define _FILE_PARSER_CONFIGURATION_READ_H_

#include "FileParserConfiguration.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserConfigurationRead
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserConfigurationRead : protected FileParserConfiguration
{
public:
  FileParserConfigurationRead (Configuration &, const String & file_name);
  ~FileParserConfigurationRead ();

protected:
  virtual void readFile (const String & file_name) final;
  virtual void registerCompiler (const String & compiler_file) final;
  virtual void activateLibrary (const String & library_name) final;
};

#endif  // _FILE_PARSER_CONFIGURATION_READ_H_
