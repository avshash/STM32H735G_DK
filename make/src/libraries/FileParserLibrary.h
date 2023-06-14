#ifndef _FILE_PARSER_LIBRARY_H_
#define _FILE_PARSER_LIBRARY_H_

#include "FileParser.h"

class Libraries;
class Library;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParserLibrary
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParserLibrary : protected FileParser
{
public:
  FileParserLibrary (Libraries &, Library &);
  ~FileParserLibrary ();

protected:
  Libraries & getLibraries ();
  Library & getLibrary ();

  virtual void registerDirective (const String & directive) final;

  virtual void registerModulePath (const String & module_path);
  virtual void registerLibrary (const String & library_path);
  virtual void registerActiveLibrary (const String & library_name);
  virtual void registerGlobalParam (const String & param_name, const String & param_value);
  virtual void registerLibraryParam (const String & param_name, const String & param_value);

private:
  void registerModulePath ();
  void registerLibrary ();
  void registerActiveLibrary ();
  void registerRedirectPath ();
  void registerGlobalParam ();
  void registerLibraryParam ();

  Libraries & m_libraries;
  Library & m_library;
};

#endif  // _FILE_PARSER_LIBRARY_H_
