#include "Common.h"
#include "H735Ltdc.h"

#include "PhysicalLtdcRegisters.h"
#include "ModuleManagerClockControl.h"
#include "PhysicalRccRegisters.h"
#include "ModuleManagerLcd.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Ltdc
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735Ltdc::H735Ltdc ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Ltdc
// @method:   setupRcc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Ltdc::setupRcc (const ModuleManagerLcd & lcd_manager)
{
  uint32_t target_frequency = lcd_manager.getParamValue (LTDC_PARAM_CLOCK_FREQUENCY);
  ModuleManagerClockControl & clock_control = ModuleManager::getClockControl ();
  clock_control.setClockFrequency (CLOCK_BUS_PLL3_R, target_frequency);

  ASSERT_TEST ((clock_control.getClockFrequency (CLOCK_BUS_PLL3_R) / 10 * 9) < target_frequency);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Ltdc
// @method:   configure
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Ltdc::configure (const ModuleManagerLcd & lcd_manager)
{
  // Enable kernel clock.
  RCC_REGISTERS.APB3ENR = RCC_REGISTERS.APB3ENR | 0x00000008UL;
  while ((RCC_REGISTERS.APB3ENR & 0x08UL) == 0);

  configureSync (lcd_manager);

  // Setup background color to white.
  LTDC_REGISTER.BCCR = 0x00FFFFFFUL;

  // Enable module.
  LTDC_REGISTER.GCR = 0x00002221UL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Ltdc
// @method:   configureSync
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Ltdc::configureSync (const ModuleManagerLcd & lcd_manager)
{
  uint32_t active_width = lcd_manager.getParamValue (LCD_PARAM_WIDTH);
  uint32_t hsync = lcd_manager.getParamValue (LTDC_PARAM_HSYNC);
  uint32_t hbp = lcd_manager.getParamValue (LTDC_PARAM_HBP);
  uint32_t hfp = lcd_manager.getParamValue (LTDC_PARAM_HFP);
  uint32_t active_height = lcd_manager.getParamValue (LCD_PARAM_HEIGHT);
  uint32_t vsync = lcd_manager.getParamValue (LTDC_PARAM_VSYNC);
  uint32_t vbp = lcd_manager.getParamValue (LTDC_PARAM_VBP);
  uint32_t vfp = lcd_manager.getParamValue (LTDC_PARAM_VFP);

  uint32_t total_width = active_width + hsync + hbp + hfp;
  uint32_t total_height = active_height + vsync + vbp + vfp;
  ASSERT_CRITICAL ((total_width < 0x00001000UL) && (total_height < 0x00000800UL));;
  ASSERT_CRITICAL ((0 < hsync) && (0 < vsync));

  LTDC_REGISTER.SSCR = ((hsync  - 1) << 16) | (vsync - 1);
  LTDC_REGISTER.BPCR = ((hsync  + hbp - 1) << 16) | (vsync + vbp - 1);
  LTDC_REGISTER.AWCR = ((hsync  + hbp + active_width - 1) << 16) | (vsync + vbp + active_height - 1);
  LTDC_REGISTER.TWCR = ((total_width - 1) << 16) | (total_height - 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Ltdc
// @method:   configureLayer1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Ltdc::configureLayer1 (TypeLtdcData ltdc_type, uint32_t width, uint32_t height)
{
  ASSERT_CRITICAL (3 < width);

  unsigned int pixel_size = configureFormat (ltdc_type);

  uint32_t h_pixel_prefix = (LTDC_REGISTER.BPCR >> 16) & 0x00000FFFUL;
  LTDC_REGISTER.L1WHPCR = ((h_pixel_prefix + width) << 16) + (h_pixel_prefix + 1);
  LTDC_REGISTER.L1CFBLR = ((width * pixel_size) << 16) | ((pixel_size * width) + 7);

  uint32_t v_pixel_prefix = LTDC_REGISTER.BPCR & 0x000007FFUL;
  LTDC_REGISTER.L1WVPCR = ((LTDC_REGISTER.AWCR & 0x000007FFUL) << 16) | (v_pixel_prefix + 1);
  LTDC_REGISTER.L1CFBLNR = height;

  LTDC_REGISTER.SRCR = 0x01UL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Ltdc
// @method:   configureFormat
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int
H735Ltdc::configureFormat (TypeLtdcData ltdc_type)
{
  switch (ltdc_type)
  {
    case LTD_DATA_RGB565:
      LTDC_REGISTER.L1PFCR = 0x02UL;
      return 2;
  }

  ASSERT_CRITICAL (false);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Ltdc
// @method:   setActiveDisplay
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Ltdc::setActiveDisplay (TypeLtdcData ltdc_type, const uint16_t * base_address, uint32_t width, uint32_t height)
{
  configureLayer1 (ltdc_type, width, height);

  LTDC_REGISTER.L1CFBAR = (uint32_t) (base_address);
  LTDC_REGISTER.L1CR = 0x01UL;
  LTDC_REGISTER.SRCR = 0x01UL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Ltdc
// @method:   verifyDisplayGood
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Ltdc::verifyDisplayGood ()
{
  ASSERT_TEST (LTDC_REGISTER.ISR == 0);
}


