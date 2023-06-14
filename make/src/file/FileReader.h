#ifndef _FILE_READER_H_
#define _FILE_READER_H_

class String;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileReader
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileReader
{
public:
  FileReader (const String & file_name);
  ~FileReader ();

  uint32_t fileSize () const;
  void readFile (char * target_buffer);

private:
  FILE * m_fp;
  uint32_t m_file_size;
};

#endif  // _FILE_READER_H_
