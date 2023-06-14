#include "Common.h"
#include "ModuleManagerLcd.h"

#include "Ltdc.h"
#include "Timer.h"
#include "TextSingleLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLcd
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerLcd::ModuleManagerLcd (Ltdc & ltdc) :
  ModuleManager (MODULE_MANAGER_LCD),
  m_ltdc (ltdc),
  m_text_manager (),
  m_active_screen (LCD_ACTIVE_ASSERT),
  m_graphics_buffer_index (0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLcd
// @method:   doAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerLcd::doAction (TypeModuleAction action)
{
  switch (action)
  {
    case MODULE_ACTION_TICK:
      m_ltdc.verifyDisplayGood ();
      break;

    case MODULE_ACTION_CONFIGURE_PLL_CLOCKS:
      m_ltdc.setupRcc (*this);
      break;

    case MODULE_ACTION_ACTIVATE_GPIO:
      m_ltdc.activateGpio ();
      break;

    case MODULE_ACTION_START:
      m_ltdc.configure (*this);
      initScreen ();
      break;

    case MODULE_ACTION_OPEN:
    case MODULE_ACTION_START_PLL_CLOCKS:
    case MODULE_ACTION_START_MDMA:
      break;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLcd
// @method:   displayNextScreen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerLcd::displayNextScreen (TypeLcdActive lcd_screen)
{
  uint32_t width = getParamValue (LCD_PARAM_WIDTH);
  uint32_t height = getParamValue (LCD_PARAM_HEIGHT);

  uint16_t * base_address = NULL;
  switch (lcd_screen)
  {
    case LCD_ACTIVE_ASSERT:
      doAssert ();
      break;

    case LCD_ACTIVE_TEXT:
      base_address = (uint16_t *) getParamValue (LCD_PARAM_SCREEN_TEXT_BASE);
      break;

    case LCD_ACTIVE_GRAPHICS:
      base_address = getNextScreenGraphics ();
      // Advance graphics buffer to next index.
      m_graphics_buffer_index = (m_graphics_buffer_index + 1) % 2;
      break;
  }

  ASSERT_CRITICAL (base_address != NULL);

  m_ltdc.setActiveDisplay (LTD_DATA_RGB565, base_address, width, height);
  m_active_screen = lcd_screen;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLcd
// @method:   initScreen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerLcd::initScreen ()
{
  m_text_manager.init ();

  TextSingleLine ().print ("Show test screen.");

  wakeupGraphicsScreen ();

  TextSingleLine ().print ("Show text screen.");
  displayNextScreen (LCD_ACTIVE_TEXT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLcd
// @method:   wakeupGraphicsScreen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerLcd::wakeupGraphicsScreen ()
{
  uint16_t * base = getNextScreenGraphics ();
  uint32_t width = (getParamValue (LCD_PARAM_WIDTH) / 16) * 16;
  uint32_t height = (getParamValue (LCD_PARAM_HEIGHT) / 16) * 16;

  for (uint32_t y = 0; y < height; y++)
  {
    for (uint32_t x = 0; x < width; x++)
    {
      uint32_t val = ((x / 16) + (y / 16)) % 4;
      switch (val)
      {
        case 0:
          *base = 0xF800;
          break;
        case 1:
          *base = 0x07E0;
          break;
        case 2:
          *base = 0x001F;
          break;
        default:
          *base = 0x0000;
          break;
      }

      if (((y < 16) && (x < 16)) || ((width - 16 < y) && (height - 16 < x)))
      {
        *base = 0xFFFF;
      }

      base++;
    }
  }

  displayNextScreen (LCD_ACTIVE_GRAPHICS);
  Common::waitMicro (1000000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLcd
// @method:   getNextScreenGraphics
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t *
ModuleManagerLcd::getNextScreenGraphics ()
{
  switch (m_graphics_buffer_index)
  {
    case 0:
      return (uint16_t *) getParamValue (LCD_PARAM_SCREEN_GRAPHICS_SCREEN_0_BASE);

    case 1:
      return (uint16_t *) getParamValue (LCD_PARAM_SCREEN_GRAPHICS_SCREEN_1_BASE);
  }

  ASSERT_CRITICAL (false);
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLcd
// @method:   registerData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerLcd::registerData (const uint16_t * buffer, uint16_t chars_count)
{
  m_text_manager.registerData (buffer, chars_count);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLcd
// @method:   doAssert
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerLcd::doAssert ()
{
  // Protect agains recursion.
  while (m_active_screen == LCD_ACTIVE_ASSERT);
  m_active_screen = LCD_ACTIVE_ASSERT;

  displayNextScreen (LCD_ACTIVE_TEXT);
  m_active_screen = LCD_ACTIVE_ASSERT;

  m_text_manager.displayAssert ();
}

