#ifndef _TYPE_USB_ENUMERATOR_STATE_H_
#define _TYPE_USB_ENUMERATOR_STATE_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeUsbEnumeratorState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeUsbEnumeratorState
{
  USB_ENUMERATOR_IDLE,
  USB_ENUMERATOR_RESET,
  USB_ENUMERATOR_WAIT_ENABLE,
  USB_ENUMERATOR_GET_MAXIMUM_PACKET_SIZE,
  USB_ENUMERATOR_SET_ADDRESS,
  USB_ENUMERATOR_GET_DEVICE_DESCRIPTOR,
  USB_ENUMERATOR_GET_CONFIGURATION,
  USB_ENUMERATOR_DEVICE_CONFIGURATION,
  USB_ENUMERATOR_ERROR
};

#endif  // _TYPE_USB_ENUMERATOR_STATE_H_
