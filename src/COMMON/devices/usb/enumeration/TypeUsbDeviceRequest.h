#ifndef _TYPE_USB_DEVICE_REQUEST_H_
#define _TYPE_USB_DEVICE_REQUEST_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeUsbDeviceRequest
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeUsbDeviceRequest
{
  USB_DEVICE_REQUEST_NONE,
  USB_DEVICE_REQUEST_GET_DESCRIPTOR,
  USB_DEVICE_REQUEST_SET_ADDRESS,
  USB_DEVICE_REQUEST_GET_CONFIGURATION,
  USB_DEVICE_REQUEST_SET_CONFIGURATION,
  USB_DEVICE_REQUEST_SET_HID_BOOT,
  USB_DEVICE_REQUEST_SET_IDLE,
  USB_DEVICE_REQUEST_HID_STATE,
  USB_DEVICE_REQUEST_UNKNOWN
};

#endif  // _TYPE_USB_DEVICE_REQUEST_H_
