#ifndef _STRING_H_
#define _STRING_H_

#include "Object.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    String
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class String
{
public:
  String ();
  String (const String & source);
  String (const char * raw_string);
  String (const char * raw_string, uint32_t string_length);

  virtual ~String ();

  char * release ();
  uint32_t hash (uint32_t limit) const;
  uint32_t stringLen () const;
  const char * cStr () const;
  bool operator== (const String &) const;
  bool operator== (const char *) const;
  bool operator!= (const String &) const;
  bool operator!= (const char *) const;
  char operator[] (int) const;
  String & operator<< (const char *);
  String & operator<< (const String &);
  String & append (const char * postfix, uint32_t postfix_len);
  void moveString (String & source);

private:
  // Not implemented.
  String (String & source);
  String & operator= (const String & other);

  uint32_t m_string_len;
  char * m_string;

  static char m_null_string[1];
};

#endif  // _STRING_H_
