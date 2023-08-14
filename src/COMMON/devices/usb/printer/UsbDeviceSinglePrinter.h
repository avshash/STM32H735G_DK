#ifndef _USB_DEVICE_SINGLE_PRINTER_H_
#define _USB_DEVICE_SINGLE_PRINTER_H_

#include "UsbDeviceSingle.h"
#include "UsbChannelSingleBulkOut.h"
#include "TypeUsbDeviceClass.h"
#include "TypeUsbPrinterState.h"

class UsbDeviceConfiguration;
class UsbChannelSingleControlDevice;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbDeviceSinglePrinter : public UsbDeviceSingle
{
public:
  TypeUsbPrinterState getPrinterState () const;

protected:
  UsbDeviceSinglePrinter ();

  void sendPrinterPacket (const uint32_t * packet, uint16_t length, TypeUsbPrinterState printer_state);

private:
  void setPrinterState (TypeUsbPrinterState);
  virtual bool testConfiguration (const UsbDeviceConfiguration &) const final;
  virtual void initiateConfiguration (UsbChannelSingleControlDevice &) final;
  virtual void registerSetupReply (UsbChannelSingleControlDevice &) final;
  virtual void deviceTick () final;
  virtual void closeDevice () final;

  virtual void clearPrinterBuffer () = 0;
  virtual void parseNextPacket (TypeUsbPrinterState) = 0;

  UsbChannelSingleBulkOut m_channel;
  TypeUsbPrinterState m_printer_state;
};

#endif  // _USB_DEVICE_SINGLE_PRINTER_H_
