#ifndef _SINGLE_PATH_DATA_H_
#define _SINGLE_PATH_DATA_H_

#include "QueueObject.h"
#include "String.h"

class Library;
class Configuration;
class Compiler;
class FileWriter;
class Module;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SinglePathData
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SinglePathData : public Object
{
public:
  SinglePathData (const Library &, const String & module_path);
  ~SinglePathData ();

  void activateSingle (Configuration & configuration, Module &);
  void addActiveObject (const Compiler &, Module & module_group, const String & base_name);
  void addObjectVariable (const char * variable_name, const String & variable_value);
  void writeCompilationRules (FileWriter & makefile) const;

  virtual TypeObject objectType () const final;

private:
  const Library & m_library;
  const String m_path;
  QueueObject m_active_objects;
};

#endif  // _SINGLE_PATH_DATA_H_
