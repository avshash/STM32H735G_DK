#include "common.h"
#include "FileParser.h"

#include <ctype.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileParser::FileParser (const String & file_name)
: m_file_buffer ("", file_name),
  m_line_number (0),
  m_error_string (NULL),
  m_cur_line (NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileParser::FileParser (const char * file_name, const String & rel_path)
: m_file_buffer (file_name, rel_path),
  m_line_number (0),
  m_error_string (NULL),
  m_cur_line (NULL)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileParser::~FileParser ()
{
  if (m_error_string != NULL)
  {
    printError ("Error in line %d, ", m_line_number, ERROR_COLOR_WHITE);
    printError ("Data: '", ERROR_COLOR_WHITE);
    printError (m_error_string, ERROR_COLOR_PURPLE);
    printError ("'. ", ERROR_COLOR_WHITE);
    printError ("In File '%s%s'.\n", m_file_buffer.filePath ().cStr (), m_file_buffer.fileName ().cStr ());
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   readNextString
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String
FileParser::readNextString (bool b_delimited)
{
  if (m_cur_line == NULL)
  {
    m_cur_line = m_file_buffer.readLine ();
    m_error_string = m_cur_line;
    m_line_number++;

    if (m_cur_line == NULL)
    {
      return String ();
    }
  }

  return generateString (b_delimited);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   resetRead
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParser::resetRead ()
{
  m_file_buffer.resetRead ();
  m_line_number = 0;
  m_error_string = NULL;
  m_cur_line = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   skipSpaces
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParser::skipSpaces ()
{
  INTERNAL_ASSERT (m_cur_line != NULL);

  do
  {
    char ch = *m_cur_line;

    if (ch == '#')
    {
      while (*m_cur_line != 0)
      {
        m_cur_line++;
      }
      break;
    }

    if ((ch == 0) || (!isspace (ch)))
    {
      break;
    }
    
    m_cur_line++;
  } while (1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   generateString
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String
FileParser::generateString (bool b_delimited)
{
  skipSpaces ();

  char tail = *m_cur_line;
  if (tail == 0)
  {
    m_cur_line = NULL;
    return String ("");
  }

  char terminating_char = 0;
  if (b_delimited && ((tail == '\"') || (tail == '\'')))
  {
    terminating_char = *m_cur_line;
    m_cur_line++;
  }

  return generateString (terminating_char);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   generateString
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String
FileParser::generateString (char terminating_char)
{
  char const * string_tail = m_cur_line;

  while (1)
  {
    unsigned char next_char = *string_tail;
    if (next_char == terminating_char)
    {
      break;
    }

    if (terminating_char == 0)
    {
      if (isspace (next_char))
      {
        break;
      }
      bool b_valid_char = isalnum (next_char) || (next_char == '_');
      VERIFY_INPUT_VALID (b_valid_char, "Undelimited illegal character used.");
    }
    else
    {
      VERIFY_INPUT_VALID ((next_char != 0), "Missing terminating delimiter.");
    }

    string_tail++;
  }

  uint32_t string_len = string_tail - m_cur_line;
  m_cur_line = ((terminating_char == 0) ? 0 : 1) + string_tail;

  return String (string_tail - string_len, string_len);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   parseFile
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
FileParser::parseFile ()
{
  while (1)
  {
    String directive = readNextString (false);
    if (directive == NULL)
    {
      break;
    }

    if (directive == "")
    {
      continue;
    }

    registerDirective (directive);

    String new_line = readNextString (false);
    VERIFY_INPUT_VALID ((new_line == ""), "Excess parameters for library directive.");
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    FileParser
// @method:   getPath
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const String &
FileParser::getPath () const
{
  return m_file_buffer.filePath ();
}

