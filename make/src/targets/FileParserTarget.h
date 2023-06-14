#ifndef _FILE_PARSER_TARGET_H_
#define _FILE_PARSER_TARGET_H_

#include "FileParser.h"
#include "QueueString.h"
#include "String.h"

class Configuration;
class Library;
class Target;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserTarget
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserTarget : protected FileParser
{
public:
  FileParserTarget (Configuration &, const String & input_file);
  ~FileParserTarget ();

protected:
  virtual void registerDirective (const String & directive) final;

private:
  void generateTarget ();
  void generateActiveObject (Target &);
  void appendDefaultParams (const Library &);
  void registerTargetName ();
  void registerLibraryName ();
  void registerTargetParam ();

  Configuration & m_configuration;
  String m_target_name;
  String m_library_name;
  QueueString m_target_params;
};

#endif  // _FILE_PARSER_TARGET_H_
