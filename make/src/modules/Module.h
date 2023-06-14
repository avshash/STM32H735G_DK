#ifndef _MODULE_H_
#define _MODULE_H_

#include "ObjectAPtr.h"
#include "QueueObject.h"
#include "QueueString.h"
#include "Object.h"
#include "String.h"

class Compiler;
class Library;
class Configuration;
class FileWriter;
class QueueString;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Module
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Module : public Object
{
public:
  Module ();
  ~Module ();

  void addSinglePathData (const Library &, const String &);
  void setModuleActive (Configuration &, const String & module_name);
  void writeModuleRule (FileWriter &) const;
  void writeCompilationRules (FileWriter &) const;
  void appendModuleParam (const char * param_name, const String & param_value);
  const QueueString & getModuleParams () const;
  bool isActive () const;

  virtual TypeObject objectType () const final;

private:
  void activateModule (Configuration & configuration);

  bool m_active;
  QueueObject m_paths_data;
  QueueString m_module_params;
};

#endif  // _MODULE_H_
