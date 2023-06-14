#ifndef _FILE_BUFFER_H_
#define _FILE_BUFFER_H_

#include "String.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileBuffer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileBuffer
{
public:
  FileBuffer (const String & file_name);
  FileBuffer (const char * file_name, const String & rel_path);
  ~FileBuffer ();

  bool endOfFile () const;
  const char * readLine ();
  void resetRead ();

  const String & filePath () const;
  const String & fileName () const;

private:
  void parseFileName (const String & file_name);
  void loadFile ();
  void parseBuffer ();

  static char getSpecialCharacter (char type_char);

  String m_file_path;
  String m_file_name;
  uint32_t m_offset;
  uint32_t m_buffer_size;
  char * m_buffer;
};

#endif  // _FILE_BUFFER_H_
