#ifndef _LIBRARY_NODE_H_
#define _LIBRARY_NODE_H_

#include "Library.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    LibraryNode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LibraryNode : public Library
{
public:
  LibraryNode (Library & parent, const String & path);
  ~LibraryNode ();

  virtual void appendSourceParams (QueueString &) const;
  virtual void addGlobalParam (const String & params_name, const String & param_value);

private:
  Library & m_parent;
};

#endif  // _LIBRARY_NODE_H_
