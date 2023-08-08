#ifndef _H735_USB_INTERRUPTS_HANDLER_H_
#define _H735_USB_INTERRUPTS_HANDLER_H_

#include "Interrupt.h"

class UsbChannelsManager;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbInterruptHandler
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735UsbInterruptHandler : public Interrupt
{
public:
  H735UsbInterruptHandler (UsbChannelsManager &);

  void configure ();

private:
  // 'Interrupt' implementation.
  virtual void handleInterrupt (TypeInterruptAction) final;
  virtual int getInterruptParam (TypeInterruptParam) final;
  virtual int getIrqNumber (int index) final;

  UsbChannelsManager & m_channels;
};

typedef H735UsbInterruptHandler PhysicalUsbInterruptHandler;

#endif  // _H735_USB_INTERRUPTS_HANDLER_H_

