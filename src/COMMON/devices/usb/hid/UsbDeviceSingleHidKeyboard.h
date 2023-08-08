#ifndef _USB_DEVICE_SINGLE_HID_KEYBOARD_H_
#define _USB_DEVICE_SINGLE_HID_KEYBOARD_H_

#include "UsbDeviceSingleHid.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHidKeyboard
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbDeviceSingleHidKeyboard : public UsbDeviceSingleHid
{
public:
  UsbDeviceSingleHidKeyboard ();

private:
  virtual TypeUsbDeviceClass getDeviceClass () const final;
  virtual uint8_t getBootReportSize () const final;
  virtual void registerInitialState (const uint8_t *) final;
  virtual void registerState (const uint8_t *) final;
};

#endif  // _USB_DEVICE_SINGLE_HID_KEYBOARD_H_
