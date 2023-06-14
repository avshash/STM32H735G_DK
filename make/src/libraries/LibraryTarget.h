#ifndef _LIBRARY_TARGET_H_
#define _LIBRARY_TARGET_H_

#include "Library.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    LibraryTarget
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LibraryTarget : public Library
{
public:
  LibraryTarget (const Library & parent, const String & path);
  ~LibraryTarget ();

  virtual const char * getDefaultModuleName () const;
  virtual const char * getObjectTargetPrefix () const;
  virtual void appendSourceVariables (QueueString &) const;

private:
  const Library & m_parent;
};

#endif  // _LIBRARY_TARGET_H_
