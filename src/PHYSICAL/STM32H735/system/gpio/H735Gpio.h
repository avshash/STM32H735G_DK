#ifndef _H735_GPIO_H_
#define _H735_GPIO_H_

#include "TypeGpioMode.h"
#include "TypeGpioSpeed.h"
#include "TypeGpioLoad.h"
#include "TypeGpioOutput.h"
#include "TypeGpioBank.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735Gpio
{
protected:
  H735Gpio (TypeGpioBank, int pin_number);
  ~H735Gpio ();

  void setPortMode (TypeGpioMode);
  void setOutputType (TypeGpioOutput);
  void setOutputSpeed (TypeGpioSpeed);
  void setLoadResistor (TypeGpioLoad load_type);
  void setAfMode (int af_index);
  void setOutput (bool b_value);
  bool readValue () const;

private:
  void setupClock ();
  void resetMode ();

  const TypeGpioBank m_bank;
  const int m_pin_index;
};

#endif  // _H735_GPIO_H_
