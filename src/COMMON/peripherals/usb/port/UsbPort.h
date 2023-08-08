#ifndef _USB_PORT_H_
#define _USB_PORT_H_

#include "TypeUsbPortState.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPort
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbPort
{
public:
  virtual TypeUsbPortState getPortState () const = 0;
  virtual void portResetAction (bool b_reset_active) = 0;
  virtual bool testLowSpeed () const = 0;
  virtual void disablePort () = 0;
};

#endif  // _USB_PORT_H_
