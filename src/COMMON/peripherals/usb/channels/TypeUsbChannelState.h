#ifndef _TYPE_USB_CHANNEL_STATE_H_
#define _TYPE_USB_CHANNEL_STATE_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeUsbChannelState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeUsbChannelState
{
  USB_CHANNEL_DETACHED,
  USB_CHANNEL_IDLE,
  USB_CHANNEL_ACTIVE,
  USB_CHANNEL_READY_IN,
  USB_CHANNEL_READY_OUT,
  USB_CHANNEL_NACK,
  USB_CHANNEL_ERROR
};

#endif  // _TYPE_USB_CHANNEL_STATE_H_
