#include "Common.h"
#include "UsbDeviceSingleHid.h"

#include "UsbDeviceConfiguration.h"
#include "UsbChannelSingleControlDevice.h"
#include "TextSingleLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHid
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbDeviceSingleHid::UsbDeviceSingleHid  () :
  UsbDeviceSingle (),
  m_channel ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHid
// @method:   testConfiguration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
UsbDeviceSingleHid::testConfiguration (const UsbDeviceConfiguration & configuration) const
{
  if (getDeviceState () != USB_DEVICE_SINGLE_CLOSED)
  {
    return false;
  }

  if (configuration.getDeviceClass () == getDeviceClass ())
  {
    if (configuration.getEndpointParams (USB_CHANNEL_ENDPOINT_INTERRUPT_IN) != NULL)
    {
      return true;
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHid
// @method:   initiateConfiguration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSingleHid::initiateConfiguration (UsbChannelSingleControlDevice & control_channel)
{
  ASSERT_TEST (getDeviceState () == USB_DEVICE_SINGLE_CLOSED);
  setDeviceState (USB_DEVICE_SINGLE_CONFIGURING);

  control_channel.controlTransaction (USB_DEVICE_REQUEST_SET_HID_BOOT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHid
// @method:   registerSetupReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSingleHid::registerSetupReply (UsbChannelSingleControlDevice & control_channel)
{
  ASSERT_TEST (control_channel.getChannelState () == USB_CHANNEL_IDLE);

  switch (control_channel.getActiveRequest ())
  {
    case USB_DEVICE_REQUEST_SET_HID_BOOT:
      control_channel.controlTransaction (USB_DEVICE_REQUEST_GET_CONFIGURATION, 0, 8);
      break;

    case USB_DEVICE_REQUEST_GET_CONFIGURATION:
      handleConfigurationReply (control_channel);
      break;

    case USB_DEVICE_REQUEST_SET_CONFIGURATION:
      control_channel.controlTransaction (USB_DEVICE_REQUEST_SET_IDLE, getIdlePeriod ());
      break;

    case USB_DEVICE_REQUEST_SET_IDLE:
      control_channel.controlTransaction (USB_DEVICE_REQUEST_HID_STATE, 0, getBootReportSize ());
      break;

    case USB_DEVICE_REQUEST_HID_STATE:
      registerInitialState (control_channel.getControlReply ());
      setDeviceState (USB_DEVICE_SINGLE_ACTIVE);
      break;

    default:
      ASSERT_TEST (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHid
// @method:   handleConfigurationReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSingleHid::handleConfigurationReply (UsbChannelSingleControlDevice & control_channel)
{
  const uint8_t * reply = control_channel.getControlReply ();

  if (control_channel.getReplySize () == 8)
  {
    uint16_t configuration_length = U8LE2U16 ((reply + 2));
    return control_channel.controlTransaction (USB_DEVICE_REQUEST_GET_CONFIGURATION, 0, configuration_length);
  }

  attachChannel (control_channel);

  control_channel.controlTransaction (USB_DEVICE_REQUEST_SET_CONFIGURATION);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHid
// @method:   handleConfigurationReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSingleHid::attachChannel (UsbChannelSingleControlDevice & control_channel)
{
  UsbDeviceConfiguration configuration (control_channel);

  const UsbEndpointParams * endpoint_params = configuration.getEndpointParams (USB_CHANNEL_ENDPOINT_INTERRUPT_IN);
  if (endpoint_params == NULL)
  {
    return setDeviceState (USB_DEVICE_SINGLE_ERROR);
  }

  m_channel.attachChannel (control_channel, endpoint_params->getInterval (), getBootReportSize ());

  m_channel.setupChannelParam (USB_CHANNEL_PARAM_MAXIMUM_PACKET_SIZE, endpoint_params->getMaximumPacketSize ());
  m_channel.setupChannelParam (USB_CHANNEL_PARAM_EP_NUMBER, endpoint_params->getEpNumber ());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHid
// @method:   deviceTick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSingleHid::deviceTick ()
{
  switch (m_channel.getChannelState ())
  {
    case USB_CHANNEL_IDLE:
      registerReport (m_channel.getReport ());
      m_channel.requestReport ();
      break;

      case USB_CHANNEL_DETACHED:
      case USB_CHANNEL_ERROR:
        setDeviceState (USB_DEVICE_SINGLE_ERROR);
        TextSingleLine ().print ("Interrupt channel closed. Closing device.");
        break;

      default:
        break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHid
// @method:   closeDevice
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSingleHid::closeDevice ()
{
  TypeUsbChannelState channel_state = m_channel.getChannelState ();

  if ((channel_state != USB_CHANNEL_DETACHED) && (channel_state != USB_CHANNEL_ERROR))
  {
    m_channel.channelError ();
  }

  // We send a 'NULL' report to notify parent that device was disconnected.
  registerReport (NULL);
}

