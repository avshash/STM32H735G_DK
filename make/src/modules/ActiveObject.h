#ifndef _ACTIVE_OBJECT_H_
#define _ACTIVE_OBJECT_H_

#include "String.h"
#include "QueueString.h"

class Compiler;
class FileWriter;
class Module;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ActiveObject
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ActiveObject : public Object
{
public:
  ActiveObject (const Compiler &);
  ~ActiveObject ();

  void expandVariables (Module &);
  void appendVariable (const char * variable_name, const String & variable_val);
  void appendVariable (const char * variable_name, const char * variable_val);
  void writeMakeRule (FileWriter &, const QueueString &) const;
  const String & getVariableValue (const String & variable_name) const;

  virtual TypeObject objectType () const final;

private:
  static void advanceUid ();

  const Compiler & m_compiler;
  QueueString m_variables_list;

  static char m_global_uid[4];
};

#endif  // _ACTIVE_OBJECT_H_
