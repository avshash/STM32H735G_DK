#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include "String.h"
#include "QueueString.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Library
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Library : public Object
{
public:
  Library (const String & path);
  ~Library ();

  void addLibraryParam (const String & variable_name, const String & variable_val);
  void addLibraryParams (const QueueString &);
  void activate ();
  bool isActive () const;
  const String & getPath () const;

  virtual const char * getDefaultModuleName () const;
  virtual const char * getObjectTargetPrefix () const;

  virtual void appendSourceParams (QueueString &) const;
  virtual void addGlobalParam (const String & variable_name, const String & variable_val);

  virtual TypeObject objectType () const;

protected:
  void appendNodeParams (QueueString &) const;

private:
  const String m_library_path;
  QueueString m_params_list;
  bool m_is_active;
};

#endif  // _LIBRARY_H_
