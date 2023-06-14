#ifndef _FILE_PARSER_CONFIGURATION_TARGET_H_
#define _FILE_PARSER_CONFIGURATION_TARGET_H_

#include "FileParserConfiguration.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserConfigurationTarget
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserConfigurationTarget : protected FileParserConfiguration
{
public:
  FileParserConfigurationTarget (Configuration &, const String & file_name);
  ~FileParserConfigurationTarget ();

protected:
  virtual void registerTarget (const String & target_file) final;
};

#endif  // _FILE_PARSER_CONFIGURATION_TARGET_H_
