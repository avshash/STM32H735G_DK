#ifndef _USB_DEVICE_SETUP_H_
#define _USB_DEVICE_SETUP_H_

#include "UsbChannelSingleControl.h"
#include "TypeUsbDeviceRequest.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControlDevice
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbChannelSingleControlDevice : public UsbChannelSingleControl
{
public:
  UsbChannelSingleControlDevice ();

  void attachChannel (UsbPort *);
  void controlTransaction (TypeUsbDeviceRequest, uint16_t override_value = 0, uint16_t reply_size = 0);

  TypeUsbDeviceRequest getActiveRequest () const;
  uint16_t getValue () const;
  virtual uint16_t getReplySize () const final;

private:
  virtual const uint32_t * getRequest () const final;

  uint32_t m_transaction[2];
};

#endif  // _USB_DEVICE_SETUP_H_
