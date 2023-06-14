#ifndef _TEXT_MANAGER_H_
#define _TEXT_MANAGER_H_

class ModuleManagerLcd;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TextManager
{
public:
  TextManager ();

  void init ();
  void registerData (const uint16_t * buffer, uint16_t chars_count);
  void displayAssert ();

private:
  void printLine (const uint16_t * buffer, uint16_t chars_count);
  void advanceActiveLine ();
  void showNextLine ();
  uint16_t * getNextTextLine ();
  uint16_t * getNextGraphicsLine ();

  static uint32_t getColor (uint16_t type_text_color);
  static uint8_t getCharMap (uint8_t char_value, int bit_line);

  uint16_t m_chars_per_line;
  uint16_t m_lines_per_screen;
  uint16_t m_text_screens_count;
  uint16_t m_next_active_line;
};

#endif  // _TEXT_MANAGER_H_
