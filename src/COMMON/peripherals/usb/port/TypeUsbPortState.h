#ifndef _TYPE_USB_PORT_STATE_H_
#define _TYPE_USB_PORT_STATE_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeUsbPortState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeUsbPortState
{
  USB_PORT_POWERED_DOWN,
  USB_PORT_DISCONNECTED,
  USB_PORT_ENUMERATION_PENDING,
  USB_PORT_RESET_ACTIVE,
  USB_PORT_CONNECTED,
  USB_PORT_ENABLED,
  USB_PORT_POWER_DOWN_PENDING
};

#endif  // _TYPE_USB_PORT_STATE_H_