#ifndef _USB_DEVICE_SINGLE_H_
#define _USB_DEVICE_SINGLE_H_

#include "TypeUsbDeviceSingleState.h"

class UsbDeviceConfiguration;
class UsbChannelSingleControlDevice;
class UsbPort;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbDeviceSingle
{
public:
  TypeUsbDeviceSingleState getDeviceState () const;
  void tick ();
  void configurationError ();
  bool testDeviceActive () const;

  virtual bool testConfiguration (const UsbDeviceConfiguration &) const = 0;
  virtual void initiateConfiguration (UsbChannelSingleControlDevice &) = 0;
  virtual void registerSetupReply (UsbChannelSingleControlDevice &) = 0;

protected:
  UsbDeviceSingle ();

  void setDeviceState (TypeUsbDeviceSingleState);

private:
  virtual void deviceTick () = 0;
  virtual void closeDevice () = 0;

  TypeUsbDeviceSingleState m_device_state;
};

#endif  // _USB_DEVICE_SINGLE_H_
