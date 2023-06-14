#ifndef _FILE_WRITER_H_
#define _FILE_WRITER_H_

class String;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileWriter
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileWriter
{
public:
  FileWriter (const String & file_name);
  ~FileWriter ();

  FileWriter & operator<< (const char *);
  FileWriter & operator<< (const String &);

private:
  FileWriter & append (const char *);

  FILE * m_fp;
};

#endif  // _FILE_WRITER_H_
