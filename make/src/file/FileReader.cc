#include "common.h"
#include "FileReader.h"

#include "String.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileReader
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileReader::FileReader (const String & file_name)
: m_fp (NULL),
  m_file_size (0)
{
  m_fp = fopen (file_name.cStr (), "r");
  if (m_fp == NULL)
  {
    printError ("Can not read file '");
    printError (file_name.cStr (), ERROR_COLOR_PURPLE);
    printError ("'.");
    VERIFY_INPUT_VALID (false, "");
  }

  fseek (m_fp, 0, SEEK_END);
  int file_size = ftell (m_fp);
  if (file_size < 0)
  {
    printError ("Can not access file '");
    printError (file_name.cStr (), ERROR_COLOR_PURPLE);
    printError ("'.");
    fclose (m_fp);
    INTERNAL_ASSERT (false);
  }

  m_file_size = file_size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileReader
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileReader::~FileReader ()
{
  if (m_fp != NULL)
  {
    fclose (m_fp);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileReader
// @method:   readFile
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileReader::readFile (char * target_buffer)
{
  rewind (m_fp);
  long read_size = fread (target_buffer, 1, m_file_size, m_fp);
  if (m_file_size != read_size)
  {
    fprintf (stderr, "Error reading file. expected size is %d, while actual size is %d.\n", (int) m_file_size, (int) read_size);
  }
  VERIFY_INPUT_VALID ((m_file_size == read_size), "Error reading file.");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileReader
// @method:   fileSize
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
FileReader::fileSize () const
{
  return m_file_size;
}

