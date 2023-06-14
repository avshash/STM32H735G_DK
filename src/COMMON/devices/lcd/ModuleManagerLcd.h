#ifndef _MODULE_MANAGER_LCD_H_
#define _MODULE_MANAGER_LCD_H_

#include "ModuleManager.h"
#include "TypeLcdParam.h"
#include "TypeLtdcParam.h"
#include "TextManager.h"
#include "TypeLcdActive.h"

class Ltdc;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerLcd
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerLcd : public ModuleManager
{
public:
  uint16_t * getNextScreenGraphics ();
  // Use virtual definition for compliation reasons.
  virtual void displayNextScreen (TypeLcdActive);
  virtual void registerData (const uint16_t * buffer, uint16_t chars_count);

  virtual uint32_t getParamValue (TypeLcdParam) const = 0;
  virtual uint32_t getParamValue (TypeLtdcParam) const = 0;

protected:
  ModuleManagerLcd (Ltdc &);

private:
  void initScreen ();
  void wakeupGraphicsScreen ();
  void doAssert ();

  virtual void doAction (TypeModuleAction) final;

  Ltdc & m_ltdc;
  TextManager m_text_manager;
  TypeLcdActive m_active_screen;
  uint8_t m_graphics_buffer_index;
};

#endif  // _MODULE_MANAGER_LCD_H_
