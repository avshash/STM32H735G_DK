#ifndef _TYPE_USB_PRINTER_STATE_H_
#define _TYPE_USB_PRINTER_STATE_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeUsbPrinterState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeUsbPrinterState
{
  USB_PRINTER_CLOSED,
  USB_PRINTER_IDLE,
  USB_PRINTER_CLEARING_BUFFER,
  USB_PRINTER_IMAGE,
  USB_PRINTER_LAST_PACKET,
  USB_PRINTER_ERROR
};

#endif  // _TYPE_USB_PRINTER_STATE_H_