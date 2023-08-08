#ifndef _USB_DEVICES_MANAGER_H_
#define _USB_DEVICES_MANAGER_H_

#include "UsbDeviceEnumerator.h"

class UsbDeviceSingle;
class UsbDeviceConfiguration;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDevicesManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbDevicesManager
{
public:
  UsbDevicesManager ();

  void registerDevice (UsbDeviceSingle &);
  void portConnected (UsbPort *);
  void tick ();
  uint8_t getFreeAddress () const;
  UsbDeviceSingle * getDeviceHandler (const UsbDeviceConfiguration &);

private:
  UsbDeviceEnumerator m_enumerator;
  UsbDeviceSingle * m_devices[8];
};

#endif  // _USB_DEVICES_MANAGER_H_
