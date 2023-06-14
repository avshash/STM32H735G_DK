#ifndef _LIBRARY_NODE_TARGET_H_
#define _LIBRARY_NODE_TARGET_H_

#include "LibraryNode.h"
#include "String.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    LibraryNodeTarget
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LibraryNodeTarget : public LibraryNode
{
public:
  LibraryNodeTarget (Library & parent);
  ~LibraryNodeTarget ();

  void setTargetName (const String & path);

  virtual const char * getDefaultModuleName () const;
  virtual const char * getObjectTargetPrefix () const;

private:
  String m_target_module_name;
  String m_target_prefix;
};

#endif  // _LIBRARY_NODE_TARGET_H_
