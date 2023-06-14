#ifndef _CLEANUP_H_
#define _CLEANUP_H_

#include "QueueString.h"
#include "SetStringObject.h"

class Target;
class QueueString;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Cleanup
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Cleanup
{
public:
  Cleanup (const SetStringObject & global_objects);
  ~Cleanup ();

private:
  void parseObjects (const SetStringObject & global_objects);
  void parseParamsList (const QueueString & variables_list);
  void registerPath (const String & file_name);
  void buildClean () const;
  void buildRemoveAll () const;

  QueueString m_clean_files;
  QueueString m_empty_files;
  SetStringObject m_paths;
};

#endif  // _CLEANUP_H_
