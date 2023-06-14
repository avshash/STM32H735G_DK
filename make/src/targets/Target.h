#ifndef _TARGET_H_
#define _TARGET_H_

#include "LibraryNodeTarget.h"
#include "QueueString.h"
#include "SetStringObject.h"

class Configuration;
class FileWriter;
class Library;
class String;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Target
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Target : public Object
{
public:
  Target (Library & parent, const QueueString & params);
  ~Target ();

  void activateTarget (Configuration &, const String & target_name);
  void writeTargetRule (FileWriter &) const;
  Library & targetLibrary ();
  const QueueString & getTargetParams () const;

  virtual TypeObject objectType () const final;

private:
  void activateModules (Configuration &, const QueueString & params_list);
  void activateModule (Configuration &, const String & module_name);

  LibraryNodeTarget m_target_library;
  QueueString m_target_params;
  SetStringObject m_active_modules;
};

#endif  // _TARGET_H_
