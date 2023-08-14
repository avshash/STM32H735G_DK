#include "Common.h"
#include "UsbDeviceSinglePrinter.h"

#include "UsbDeviceConfiguration.h"
#include "UsbChannelSingleControlDevice.h"
#include "TextSingleLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbDeviceSinglePrinter::UsbDeviceSinglePrinter  () :
  UsbDeviceSingle (),
  m_printer_state (USB_PRINTER_CLOSED)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
// @method:   setPrinterState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinter::setPrinterState (TypeUsbPrinterState new_state)
{
  ASSERT_TEST (m_printer_state != new_state);

  m_printer_state = new_state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
// @method:   testConfiguration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
UsbDeviceSinglePrinter::testConfiguration (const UsbDeviceConfiguration & configuration) const
{
  if (getDeviceState () != USB_DEVICE_SINGLE_CLOSED)
  {
    return false;
  }

  if (configuration.getDeviceClass () == USB_DEVICE_CLASS_PRINTER)
  {
    if (configuration.getEndpointParams (USB_CHANNEL_ENDPOINT_BULK_OUT) != NULL)
    {
      return true;
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
// @method:   initiateConfiguration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinter::initiateConfiguration (UsbChannelSingleControlDevice & control_channel)
{
  UsbDeviceConfiguration configuration (control_channel);

  const UsbEndpointParams * endpoint_params = configuration.getEndpointParams (USB_CHANNEL_ENDPOINT_BULK_OUT);
  if (endpoint_params == NULL)
  {
    ASSERT_TEST (false);
    return;
  }

  ASSERT_TEST (getDeviceState () == USB_DEVICE_SINGLE_CLOSED);
  setDeviceState (USB_DEVICE_SINGLE_CONFIGURING);

  m_channel.attachChannel (control_channel);

  m_channel.setupChannelParam (USB_CHANNEL_PARAM_MAXIMUM_PACKET_SIZE, endpoint_params->getMaximumPacketSize ());
  m_channel.setupChannelParam (USB_CHANNEL_PARAM_EP_NUMBER, endpoint_params->getEpNumber ());

  control_channel.controlTransaction (USB_DEVICE_REQUEST_SET_CONFIGURATION);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
// @method:   registerSetupReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinter::registerSetupReply (UsbChannelSingleControlDevice & control_channel)
{
  switch (control_channel.getActiveRequest ())
  {
    case USB_DEVICE_REQUEST_SET_CONFIGURATION:
      control_channel.controlTransaction (USB_DEVICE_REQUEST_SET_PRINTER_SOFT_RESET);
      break;

    case USB_DEVICE_REQUEST_SET_PRINTER_SOFT_RESET:
      setDeviceState (USB_DEVICE_SINGLE_ACTIVE);
      setPrinterState (USB_PRINTER_IDLE);
      clearPrinterBuffer ();
      break;

    default:
      ASSERT_TEST (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
// @method:   sendPrinterPacket
// @note:     When channel is closed (or in error state), the channel would ignore this request.
// @note:     The device would be closed by the 'deviceTick' method, that tests the channel state.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinter::sendPrinterPacket (const uint32_t * packet, uint16_t length, TypeUsbPrinterState printer_state)
{
  ASSERT_TEST (m_printer_state == USB_PRINTER_IDLE);

  m_channel.initTransaction (packet, length);

  setPrinterState (printer_state);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
// @method:   deviceTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinter::deviceTick ()
{
  ASSERT_TEST (getDeviceState () == USB_DEVICE_SINGLE_ACTIVE);
  ASSERT_TEST ((m_printer_state != USB_PRINTER_CLOSED) && (m_printer_state != USB_PRINTER_ERROR));

  switch (m_channel.getChannelState ())
  {
    case USB_CHANNEL_IDLE:
      if (m_printer_state != USB_PRINTER_IDLE)
      {
        TypeUsbPrinterState printer_state = m_printer_state;
        setPrinterState (USB_PRINTER_IDLE);
        parseNextPacket (printer_state);
      }
      break;

    case USB_CHANNEL_DETACHED:
    case USB_CHANNEL_ERROR:
      TextSingleLine ().print ("Printer bulk channel closed. Closing device.");
      setDeviceState (USB_DEVICE_SINGLE_ERROR);
      break;

    default:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
// @method:   closeDevice
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinter::closeDevice ()
{
  TypeUsbChannelState channel_state = m_channel.getChannelState ();

  if ((channel_state != USB_CHANNEL_DETACHED) && (channel_state != USB_CHANNEL_ERROR))
  {
    m_channel.channelError ();
  }

  ASSERT_TEST (getDeviceState () == USB_DEVICE_SINGLE_ERROR);
  setPrinterState (USB_PRINTER_CLOSED);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinter
// @method:   getPrinterState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeUsbPrinterState
UsbDeviceSinglePrinter::getPrinterState () const
{
  return m_printer_state;
}

