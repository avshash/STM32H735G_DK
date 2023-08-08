#ifndef _USB_DEVICE_ENUMERATOR_H_
#define _USB_DEVICE_ENUMERATOR_H_

#include "UsbChannelSingleControlDevice.h"
#include "TypeUsbEnumeratorState.h"
#include "TypeUsbDeviceRequest.h"
#include "Timer.h"

class UsbPort;
class UsbDeviceSingle;
class UsbDeviceConfiguration;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// AVSHASH:   Split to two classes. One for enabling channel, and the other for configuring device.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbDeviceEnumerator
{
public:
  UsbDeviceEnumerator ();

  void portConnected (UsbPort *);
  void tick ();

private:
  void setState (TypeUsbEnumeratorState);
  void tickReset ();
  void tickWaitEnable ();
  void waitErrorState ();
  void parseDeviceReply ();
  void parseGetMaximumPacketSize ();
  void setAddressAck ();
  void parseDeviceDescriptor ();
  void parseConfiguration ();
  void setConfiguringDevice (const UsbDeviceConfiguration &);
  void tickDeviceConfiguration ();
  void sendControlRequest (TypeUsbDeviceRequest request, uint16_t value, uint16_t reply_size, uint32_t timeout);
  bool waitControlReply ();
  void registerDeviceReady ();
  void registerControlError (const char * error_message);

  UsbChannelSingleControlDevice m_device_setup;
  TypeUsbEnumeratorState m_state;
  UsbPort * m_active_port;
  UsbDeviceSingle * m_configuring_device;
  Timer m_timer;
};

#endif  // _USB_DEVICE_ENUMERATOR_H_
