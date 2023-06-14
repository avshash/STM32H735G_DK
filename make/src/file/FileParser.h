#ifndef _FILE_PARSER_H_
#define _FILE_PARSER_H_

#include "FileBuffer.h"
#include "String.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileParser
{
protected:
  FileParser (const String & file_name);
  FileParser (const char * file_name, const String & rel_path);
  ~FileParser ();

  void parseFile ();
  String readNextString (bool b_delimited);
  void resetRead ();
  const String & getPath () const;

  virtual void registerDirective (const String & directive) = 0;

private:
  void skipSpaces ();
  String generateString (bool b_delimited);
  String generateString (char terminating_char);

  FileBuffer m_file_buffer;
  uint32_t m_line_number;
  char const * m_error_string;
  char const * m_cur_line;
};

#endif  // _FILE_PARSER_H_
