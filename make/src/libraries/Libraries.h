#ifndef _LIBRARIES_H_
#define _LIBRARIES_H_

#include "SetStringObject.h"
#include "QueueString.h"

class Library;
class Configuration;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Libraries
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Libraries
{
public:
  Libraries (Configuration &);
  ~Libraries ();

  void registerLibrary (const String & library_name, ObjectAPtr new_library);
  void activateLibrary (const String & library_key);
  void registerModule (const Library & library, const String & module_path);

private:
  Configuration & m_configuration;
};

#endif  // _LIBRARIES_H_
