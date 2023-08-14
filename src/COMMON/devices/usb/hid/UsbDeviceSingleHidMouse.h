#ifndef _USB_DEVICE_SINGLE_HID_MOUSE_H_
#define _USB_DEVICE_SINGLE_HID_MOUSE_H_

#include "UsbDeviceSingleHid.h"
#include "UsbMouseState.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHidMouse
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbDeviceSingleHidMouse : public UsbDeviceSingleHid
{
public:
  UsbDeviceSingleHidMouse ();

  uint16_t getXLocation () const;
  uint16_t getYLocation () const;
  bool testRightButton () const;
  bool testLeftButton () const;

private:
  virtual TypeUsbDeviceClass getDeviceClass () const final;
  virtual uint8_t getBootReportSize () const final;
  virtual void registerReport (const uint8_t *) final;
  virtual uint16_t getIdlePeriod () const final;

  UsbMouseState m_state;
};

#endif  // _USB_DEVICE_SINGLE_HID_MOUSE_H_
