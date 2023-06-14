#include "Common.h"
#include "TextManager.h"

#include "ModuleManagerLcd.h"
#include "TypeTextColor.h"
#include "SystemTime.h"
#include "U8_FONT_6_8_MAP.h"
#include "TextSingleLine.h"
#include "PhysicalCriticalSection.h"
#include <cstring>

// Add 'getNextPhysicalLine'

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TextManager::TextManager () :
  m_chars_per_line (0),
  m_lines_per_screen (0),
  m_text_screens_count (0),
  m_next_active_line (0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   init
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
TextManager::init ()
{
  const ModuleManagerLcd & lcd_manager = ModuleManager::getLcd ();

  // Setup params.
  m_chars_per_line = lcd_manager.getParamValue (LCD_PARAM_WIDTH) / 6;
  m_lines_per_screen = lcd_manager.getParamValue (LCD_PARAM_HEIGHT) / 8;

  uint32_t bytes_per_screen = m_chars_per_line * m_lines_per_screen * 2;
  uint32_t text_screens_count_limit = lcd_manager.getParamValue (LCD_PARAM_TEXT_BUFFER_BYTES_SIZE) / bytes_per_screen;
  m_text_screens_count = MIN (text_screens_count_limit, (0xFFFFU / m_lines_per_screen));
  ASSERT_TEST (0 < m_text_screens_count);

  // Init screen buffer to hold an empty screen.
  uint16_t * text_base = getNextTextLine ();
  memset (text_base, 0, m_chars_per_line * 2);

  uint16_t dummy_char = 0;
  while (m_next_active_line < m_lines_per_screen)
  {
    registerData (&dummy_char, 1);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   registerData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
TextManager::registerData (const uint16_t * buffer, uint16_t chars_count)
{
  ASSERT_TEST (0 < chars_count);

  uint32_t active_chars_per_line = m_chars_per_line - 2;

  for (uint16_t offset = 0; offset < chars_count; offset += active_chars_per_line)
  {
    uint16_t reminder = chars_count - offset;
    reminder = MIN (reminder, active_chars_per_line);
    printLine (buffer + offset, reminder);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   printLine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
TextManager::printLine (const uint16_t * text_buffer, uint16_t chars_count)
{
  uint16_t * line_start = getNextTextLine ();
  ASSERT_CRITICAL (chars_count <= (m_chars_per_line - 2));

  line_start[0] = 0x68;
  memcpy (line_start + 1, text_buffer, 2 * chars_count);
  memset (line_start + 1 + chars_count, 0, 2 * (m_chars_per_line - 2 - chars_count));
  line_start[m_chars_per_line - 1] = 0x64;
  showNextLine ();

  advanceActiveLine ();

  line_start = getNextTextLine ();
  line_start[0] = TEXT_COLOR_GREEN_BLACK | 0x68;
  for (uint16_t offset = 1; offset < m_chars_per_line - 1; offset++)
  {
    line_start[offset] = TEXT_COLOR_GREEN_BLACK | 0x69;
  }
  line_start[m_chars_per_line - 1] = TEXT_COLOR_GREEN_BLACK | 0x64;
  showNextLine ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   advanceActiveLine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
TextManager::advanceActiveLine ()
{
  m_next_active_line++;

  // In case of a new screen, register frame bottom line, and frame top line.
  if ((m_next_active_line % m_lines_per_screen) == (m_lines_per_screen - 1))
  {
    uint16_t * line_start = getNextTextLine ();
    line_start[0] = 0x62;
    for (uint16_t offset = 1; offset < m_chars_per_line - 1; offset++)
    {
      line_start[offset] = 0x66;
    }
    line_start[m_chars_per_line - 1] = 0x63;
    showNextLine ();

    m_next_active_line++;

    line_start = getNextTextLine ();
    line_start[0] = 0x61;
    for (uint16_t offset = 1; offset < m_chars_per_line - 1; offset++)
    {
      line_start[offset] = 0x67;
    }
    line_start[m_chars_per_line - 1] = 0x60;
    showNextLine ();

    m_next_active_line++;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   getNextTextLine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t *
TextManager::getNextTextLine ()
{
  const ModuleManagerLcd & lcd_manager = ModuleManager::getLcd ();
  uint16_t * text_base = (uint16_t *) (lcd_manager.getParamValue (LCD_PARAM_TEXT_BUFFER_BASE));

  uint32_t lines_in_text_array = m_text_screens_count * m_lines_per_screen;
  uint32_t actual_line_index = m_next_active_line % lines_in_text_array;
  uint32_t array_offset = actual_line_index * m_chars_per_line;

  return (text_base + array_offset);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   showNextLine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
TextManager::showNextLine ()
{
  const uint16_t * text_buffer = getNextTextLine ();
  uint16_t * grpahics_target = getNextGraphicsLine ();

  for (int bit_line = 0; bit_line < 8; bit_line++)
  {
    for (uint16_t line_offset = 0; line_offset < m_chars_per_line; line_offset++)
    {
      uint16_t next_char_value = text_buffer[line_offset];
      uint8_t bit_data = getCharMap (next_char_value & 0x00FF, bit_line);
      uint32_t char_color = getColor (next_char_value & 0xFF00);

      for (uint8_t mask = 0x20; 0 < mask; mask = mask >> 1)
      {
        *grpahics_target = ((bit_data & mask) != 0) ? (char_color & 0x0000FFFFUL) : (char_color >> 16);
        grpahics_target++;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   getNextGraphicsLine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t *
TextManager::getNextGraphicsLine ()
{
  const ModuleManagerLcd & lcd_manager = ModuleManager::getLcd ();
  uint16_t * graphics_base = (uint16_t *) (lcd_manager.getParamValue (LCD_PARAM_SCREEN_TEXT_BASE));

  uint16_t actual_line = m_next_active_line % m_lines_per_screen;
  uint32_t array_offset = actual_line * 6 * 8 * m_chars_per_line;

  return (graphics_base + array_offset);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   getColor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
TextManager::getColor (uint16_t type_text_color)
{
  switch ((TypeTextColor) type_text_color)
  {
    case TEXT_COLOR_WHITE_BLACK:
      return 0x0000FFFFUL;

    case TEXT_COLOR_RED_BLACK:
      return 0x0000F800UL;

    case TEXT_COLOR_GREEN_BLACK:
      return 0x000007E0UL;

    case TEXT_COLOR_BLUE_BLACK:
      return 0x0000001FUL;

    case TEXT_COLOR_PURPLE_BLACK:
      return 0x0000F81FUL;

    case TEXT_COLOR_AZURE_BLACK:
      return 0x000007FFUL;

    case TEXT_COLOR_BLACK_WHITE:
      return 0xFFFF0000UL;
  }

  ASSERT_TEST (false);
  return 0x0000FFFFUL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   getCharMap
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t
TextManager::getCharMap (uint8_t char_value, int bit_line)
{
  const uint8_t char_map[] = U8_FONT_6_8_MAP;

  ASSERT_TEST (char_value <= (sizeof (char_map) / 8));
  char_value = MIN (char_value, ((sizeof (char_map) / 8) - 1));

  uint32_t char_map_offset = 8 * (uint32_t) char_value;

  return char_map[char_map_offset + bit_line];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
// @method:   displayAssert
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
TextManager::displayAssert ()
{
  // Disable interrupts, except for the systick.
  PhysicalCriticalSection critical_section;

  uint32_t last_active_line = m_next_active_line;
  uint32_t active_screens = MIN (10, m_text_screens_count) - 2;
  uint32_t first_active_line = m_next_active_line % m_lines_per_screen;
  if ((active_screens * m_lines_per_screen) < (m_next_active_line - first_active_line))
  {
    first_active_line = m_next_active_line - (active_screens * m_lines_per_screen);
  }

  uint32_t dummy_screen_index = (m_next_active_line / m_lines_per_screen) + 1;
  uint32_t dummy_seperators_base = dummy_screen_index * m_lines_per_screen;
  for (uint32_t line_offset = 0; line_offset < m_lines_per_screen; line_offset++)
  {
    m_next_active_line = dummy_seperators_base + line_offset;
    uint16_t * line_start = getNextTextLine ();
    line_start[0] = TEXT_COLOR_PURPLE_BLACK | 0x68;
    for (uint16_t offset = 1; offset < m_chars_per_line - 1; offset++)
    {
      line_start[offset] = TEXT_COLOR_PURPLE_BLACK | 0x69;
    }
    line_start[m_chars_per_line - 1] = TEXT_COLOR_PURPLE_BLACK | 0x64;
  }

  while (last_active_line != 0xFFFFFFFFUL)
  {
    m_next_active_line = last_active_line;
    for (uint16_t lines_count = 0; lines_count < m_lines_per_screen; lines_count++)
    {
      showNextLine ();
      m_next_active_line--;
    }

    Common::waitMicro (2000000);
    for (uint16_t active_line = m_next_active_line; first_active_line < active_line; active_line--)
    {
      m_next_active_line = active_line;
      showNextLine ();
      m_next_active_line = dummy_seperators_base + ((active_line - 1) % m_lines_per_screen);
      showNextLine ();
      Common::waitMicro (100000);
    }
    Common::waitMicro (500000);
  }
}



