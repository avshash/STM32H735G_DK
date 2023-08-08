#ifndef _USB_CHANNEL_SINGLE_H_
#define _USB_CHANNEL_SINGLE_H_

#include "TypeUsbChannelState.h"
#include "TypeUsbChannelParam.h"
#include "TypeUsbChannelEndpoint.h"

class UsbPort;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbChannelSingle
{
public:
  // Following methods are never called inside an otg interrupt handler.
  void channelError ();
  void detachChannel ();
  uint16_t getChannelParam (TypeUsbChannelParam) const;
  void setupChannelParam (TypeUsbChannelParam, uint16_t new_value);
  bool tick (TypeUsbChannelState);
  TypeUsbChannelState getChannelState () const;
  UsbPort * getUsbPort ();

  // Following methods are never called inside an otg interrupt handler.
  virtual void notifyTransactionComplete (TypeUsbChannelState) = 0;

  // Following methods may be called inside an otg interrupt handler. Implement efficiently.
  virtual void nextPacketOut (uint32_t * buffer, uint16_t bytes_count, uint16_t bytes_offset) = 0;
  virtual void nextPacketIn (const uint32_t * buffer, uint16_t bytes_count, uint16_t bytes_offset) = 0;

protected:
  UsbChannelSingle ();

  // Following methods are never called inside an otg interrupt handler.
  void attachChannel (TypeUsbChannelEndpoint, UsbPort *);
  void sendRequest (uint16_t request_length, uint8_t enable_delay = 1);
  void receiveReply (uint16_t reply_length, uint8_t enable_delay = 1);

private:
  int m_channel_index;
};

#endif  // _USB_CHANNEL_SINGLE_H_
