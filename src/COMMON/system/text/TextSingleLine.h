#ifndef _TEXT_SINGLE_LINE_H
#define _TEXT_SINGLE_LINE_H

#include "TypeTextColor.h"
#include "TypeTextStyle.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    TextSingleLine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TextSingleLine
{
public:
  TextSingleLine (TypeTextColor color = TEXT_COLOR_WHITE_BLACK, TypeTextStyle style = TEXT_STYLE_DEC);
  ~TextSingleLine ();

  // User interface.
  TextSingleLine & print (const char *);
  TextSingleLine & print (uint32_t);
  TextSingleLine & setNumStyle (TypeTextStyle);
  TextSingleLine & setColor (TypeTextColor);

private:
  void flushLine ();
  TextSingleLine & printUBase (uint32_t number, uint8_t digits, uint8_t base);

  TypeTextColor m_color;
  TypeTextStyle m_style;
  uint8_t m_cur_offset;
  uint16_t m_buffer[128];
};

TextSingleLine & operator<< (TextSingleLine &, const char *);
TextSingleLine & operator<< (TextSingleLine &, uint32_t);
TextSingleLine & operator<< (TextSingleLine &, TypeTextStyle);
TextSingleLine & operator<< (TextSingleLine &, TypeTextColor);

#endif  // _TEXT_SINGLE_LINE_H
