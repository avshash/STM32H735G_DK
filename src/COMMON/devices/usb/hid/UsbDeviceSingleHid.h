#ifndef _USB_DEVICE_SINGLE_HID_H_
#define _USB_DEVICE_SINGLE_HID_H_

#include "UsbDeviceSingle.h"
#include "UsbChannelSingleInterrupt.h"
#include "TypeUsbDeviceClass.h"

class UsbDeviceConfiguration;
class UsbChannelSingleControlDevice;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHid
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbDeviceSingleHid : public UsbDeviceSingle
{
protected:
  UsbDeviceSingleHid ();

private:
  void handleConfigurationReply (UsbChannelSingleControlDevice &);
  void attachChannel (UsbChannelSingleControlDevice &);

  virtual bool testConfiguration (const UsbDeviceConfiguration &) const final;
  virtual void initiateConfiguration (UsbChannelSingleControlDevice &) final;
  virtual void registerSetupReply (UsbChannelSingleControlDevice &) final;
  virtual void deviceTick () final;
  virtual void closeDevice () final;

  virtual TypeUsbDeviceClass getDeviceClass () const = 0;
  virtual uint8_t getBootReportSize () const = 0;
  virtual void registerInitialState (const uint8_t *) = 0;
  virtual void registerReport (const uint8_t *) = 0;
  virtual uint8_t getIdlePeriod () const = 0;

  UsbChannelSingleInterrupt m_channel;
};

#endif  // _USB_DEVICE_SINGLE_HID_H_
