#ifndef _USB_DEVICE_SINGLE_HID_PRINTER_ESC_H_
#define _USB_DEVICE_SINGLE_HID_PRINTER_ESC_H_

#include "UsbDeviceSinglePrinter.h"
#include "UsbPrintFlashData.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbDeviceSinglePrinterEsc : public UsbDeviceSinglePrinter
{
public:
  UsbDeviceSinglePrinterEsc ();

  void printRawData (uint8_t x_size, uint16_t y_size, uint32_t flash_address_base);
  void printSingleLine (const char * text);
  void printCutPaper ();
  void printQrCode (const char * qr_code, uint16_t qr_code_length);

private:
  void printClearingBuffer ();
  void printImage ();
  void printImageBody ();
  void printImageTail ();
  void printImageHeader ();

  virtual void clearPrinterBuffer () final;
  virtual void parseNextPacket (TypeUsbPrinterState) final;

  UsbPrintFlashData m_flash_data;
  uint32_t m_buffer[32];
};

#endif  // _USB_DEVICE_SINGLE_HID_PRINTER_ESC_H_
