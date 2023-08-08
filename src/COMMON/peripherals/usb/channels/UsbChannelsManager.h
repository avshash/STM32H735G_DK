#ifndef _USB_CHANNELS_MANAGER_H_
#define _USB_CHANNELS_MANAGER_H_

#include "UsbChannelData.h"
#include "TypeUsbChannelEndpoint.h"
#include "TypeUsbChannelParam.h"

class UsbChannelSingle;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbChannelsManager
{
public:
  void tick ();

  // Following methods are never called inside an otg interrupt handler. Called by 'UsbChannelSingle'.
  int attachChannel (TypeUsbChannelEndpoint, UsbChannelSingle *, UsbPort *);
  void detachChannel (int channel_index);
  void sendRequest (int channel_index, uint16_t request_length, uint8_t delay);
  void receiveReply (int channel_index, uint16_t reply_length, uint8_t delay);
  UsbPort * getUsbPort (int channel_index);
  TypeUsbChannelState getChannelState (int channel_index) const;
  virtual void setupChannelParam (int channel_index, TypeUsbChannelParam, uint16_t new_value) = 0;

  // Following methods may be called inside an otg interrupt handler.
  virtual uint16_t getChannelParam (int channel_index, TypeUsbChannelParam) const = 0;

  // Following methods are called inside an otg interrupt handler. Implement efficiently.
  virtual void markSof () = 0;
  virtual void handleRxPacket () = 0;
  
protected:
  UsbChannelsManager ();

  // Following methods may be called inside an otg interrupt handler.
  void setChannelState (int channel_index, TypeUsbChannelState channel_state);

  // Following methods are called inside an otg interrupt handler. Implement efficiently.
  void sendNextPacket (int channel_index, uint32_t * target);
  void receiveNextPacket (int channel_index, const uint32_t * source, uint16_t packet_size);

private:
  // Following methods are never called inside an otg interrupt handler.
  void tickChannel (int channel_index);
  void setChannelError (int channel_index, const char * error_message);
  TypeUsbChannelState testTransactionResult (int channel_index);
  void setupChannelTransaction (int channel_index, uint16_t reply_length, uint8_t delay, bool b_direction_in);
  virtual void initChannelTransaction (int channel_index, uint16_t data_size,
                                       uint8_t enable_delay, bool b_direction_in) = 0;
  virtual void disableChannel (int channel_index) = 0;
  virtual void setupChannel (int channel_index, TypeUsbChannelEndpoint) = 0;

  UsbChannelData m_active_channels[8];
};

#endif  // _USB_CHANNELS_MANAGER_H_
