#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "SetStringObject.h"
#include "QueueString.h"
#include "Libraries.h"

class FileWriter;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Configuration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Configuration
{
public:
  Configuration (const String & root_file_name);
  ~Configuration ();

  bool testObjectKey (const String & key) const;
  Object & getObject (const String & key);
  const Object & getObject (const String & key) const;
  void addObject (const String & key, ObjectAPtr object);
  void addModuleBinFiles (const String & module_name, ObjectAPtr bin_files);
  void registerActiveLibrary (const String & library_name);
  const QueueString & getGlobalVariableList () const;

private:
  void readConfiguration (const String & root_file_name);
  void readTargets (const String & root_file_name);
  void activateTargets ();
  void writeMainMakefile ();
  void writeMakefileHeader (FileWriter &) const;
  void writeBlockHeader (FileWriter &, const char * remark) const;
  void writeGeneralRules (FileWriter &) const;
  void writeRules (FileWriter &) const;
  void writeTargetRules (FileWriter &) const;
  void writeModuleRules (FileWriter &) const;
  void writeCompilationRules (FileWriter &) const;
  void buildCleanup () const;

  SetStringObject m_objects;
  Libraries m_libraries;
};

#endif  // _CONFIGURATION_H_
