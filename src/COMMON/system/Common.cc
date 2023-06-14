#include "Common.h"

#include "ModuleManagerClockControl.h"
#include "ModuleManagerLeds.h"
#include "ModuleManagerLcd.h"
#include "TextSingleLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @function: assertPrint
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Common::assertPrint (const char * file_name, uint32_t line_number, uint32_t unique_id)
{
  if (ModuleManager::testModuleActive (MODULE_MANAGER_LEDS))
  {
    ModuleManagerLeds & leds_manager = ModuleManager::getLeds ();

    leds_manager.setLed (LED_COLOR_RED, true);
    leds_manager.setLed (LED_COLOR_GREEN, false);
  }

  if ((unique_id == 0) || (!ModuleManager::testModuleActive (MODULE_MANAGER_LCD)))
  {
    // Need the use of a debugger to id the exact assert source.
    while (unique_id != 0xFFFFFFFFUL);
  }

  {
    TextSingleLine msg (TEXT_COLOR_RED_BLACK);
    msg << TEXT_COLOR_WHITE_BLACK << "Assert failure on " << TEXT_COLOR_RED_BLACK << file_name;
    msg << TEXT_COLOR_WHITE_BLACK << " at line " << TEXT_COLOR_RED_BLACK << line_number;
  }

  if (ModuleManager::testModuleActive (MODULE_MANAGER_LCD))
  {
    ModuleManagerLcd & lcd_manager = ModuleManager::getLcd ();
    lcd_manager.displayNextScreen (LCD_ACTIVE_ASSERT);
  }

  while (1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @function: log2int
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int
Common::log2int (uint32_t variable)
{
  // Assert power of 2.
  ASSERT_DEV ((variable & (variable - 1)) == 0);
  
  if (0x80000000UL < variable)
  {
    return 32;
  }

  unsigned int log_value = 0;
  while ((0x01UL << log_value) < variable)
  {
    log_value++;
  }

  return log_value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @function: waitMicro
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Common::waitMicro (uint32_t milli_wait)
{
  ModuleManagerClockControl & rcc_manager = ModuleManager::getClockControl ();

  rcc_manager.waitMicro (milli_wait);
}


