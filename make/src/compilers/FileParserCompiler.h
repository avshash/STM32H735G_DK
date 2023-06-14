#ifndef _FILE_PARSER_COMPILER_H_
#define _FILE_PARSER_COMPILER_H_

#include "FileParser.h"
#include "String.h"

class Configuration;
class Compiler;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserCompiler
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserCompiler : protected FileParser
{
public:
  FileParserCompiler (Configuration &, const String & soruce_file);
  ~FileParserCompiler ();

protected:
  virtual void registerDirective (const String & directive) final;

private:
  void registerConfiguration ();
  void registerName ();
  void registerVariableRule ();
  void registerModuleParam ();
  void registerMakeRuleConst ();
  void registerMakeRuleParam ();


  Configuration & m_configuration;
  Compiler * m_compiler;
  String m_compiler_name;
};

#endif  // _FILE_PARSER_COMPILER_H_
