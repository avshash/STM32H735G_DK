#ifndef _FILE_PARSER_MODULE_ACTIVATE_H_
#define _FILE_PARSER_MODULE_ACTIVATE_H_

#include "FileParserModule.h"

class Configuration;
class Library;
class String;
class SinglePathData;
class Module;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserModuleActivate
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserModuleActivate : protected FileParserModule
{
public:
  FileParserModuleActivate (Configuration &, Module &, const String & path, SinglePathData &);
  ~FileParserModuleActivate ();

protected:
  virtual void registerModuleParam (const String & param_name, const String & param_value) final;
  virtual void registerFile (const String & compiler_name, const String & base_name) final;

private:
  SinglePathData & m_single;
  Module & m_module;
};

#endif  // _FILE_PARSER_MODULE_ACTIVATE_H_
