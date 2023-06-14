#ifndef _FILE_PARSER_MODULE_H_
#define _FILE_PARSER_MODULE_H_

#include "FileParser.h"

class Configuration;
class Module;
class String;
class Compiler;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModule
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserModule : protected FileParser
{
public:
  static const char * defaultModuleName ();

protected:
  FileParserModule (Configuration &, const String & path);
  ~FileParserModule ();

  virtual void registerDirective (const String & directive) final;

  virtual void registerModuleName (const String & module_name);
  virtual void registerModule (const String & module_path);
  virtual void registerModuleParam (const String & param_name, const String & param_value);
  virtual void registerFile (const String & compiler_name, const String & base_name);

  Module & getModule (const String & module_name, bool b_create_new_module);
  const Compiler & getCompiler (const String & compiler_name) const;
  Configuration & getConfiguration ();

private:
  void registerModuleName ();
  void registerModule ();
  void registerModuleParam ();
  void registerFile ();

  Configuration & m_configuration;
};

#endif  // _FILE_PARSER_MODULE_H_
