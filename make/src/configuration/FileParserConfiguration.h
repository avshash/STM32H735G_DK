#ifndef _FILE_PARSER_CONFIGURATION_H_
#define _FILE_PARSER_CONFIGURATION_H_

#include "FileParser.h"

class Configuration;
class String;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserConfiguration
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserConfiguration : protected FileParser
{
public:
  FileParserConfiguration (Configuration &, const String & file_name);
  ~FileParserConfiguration ();

protected:
  Configuration & getConfiguration ();
  virtual void registerDirective (const String & directive) final;

  virtual void readFile (const String & read_file);
  virtual void registerCompiler (const String & compiler_file);
  virtual void registerTarget (const String & target_file);
  virtual void activateLibrary (const String & library_file);

private:
  void readFile ();
  void registerCompiler ();
  void registerTarget ();
  void activateLibrary ();

  Configuration & m_configuration;
};

#endif  // _FILE_PARSER_CONFIGURATION_H_
