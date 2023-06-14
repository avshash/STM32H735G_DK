#include "Common.h"
#include "Disc735ModuleManagerLcd.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerLcd
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Disc735ModuleManagerLcd::Disc735ModuleManagerLcd () :
  ModuleManagerLcd (m_ltdc),
  m_ltdc ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerLcd
// @method:   getParamValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
Disc735ModuleManagerLcd::getParamValue (TypeLcdParam param) const
{
  switch (param)
  {
    case LCD_PARAM_SCREEN_TEXT_BASE:
      return 0x70000000UL;

    case LCD_PARAM_SCREEN_GRAPHICS_SCREEN_0_BASE:
      return 0x70040000UL;

    case LCD_PARAM_SCREEN_GRAPHICS_SCREEN_1_BASE:
      return 0x70080000UL;

    case LCD_PARAM_TEXT_BUFFER_BASE:
      return 0x70100000UL;

    case LCD_PARAM_TEXT_BUFFER_BYTES_SIZE:
      return 0x00100000UL;

    case LCD_PARAM_WIDTH:
      return 480;

    case LCD_PARAM_HEIGHT:
      return 272;
  }

  ASSERT_CRITICAL (false);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerLcd
// @method:   getParamValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
Disc735ModuleManagerLcd::getParamValue (TypeLtdcParam param) const
{
  switch (param)
  {
    case LTDC_PARAM_CLOCK_FREQUENCY:
      return 9100000;

    case LTDC_PARAM_HSYNC:
      return 1;

    case LTDC_PARAM_HBP:
      return 42;

    case LTDC_PARAM_HFP:
      return 8;

    case LTDC_PARAM_VSYNC:
      return 10;

    case LTDC_PARAM_VBP:
      return 2;

    case LTDC_PARAM_VFP:
      return 4;
  }

  ASSERT_CRITICAL (false);
  return 0;
}


