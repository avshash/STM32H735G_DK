#include "Common.h"
#include "UsbDeviceEnumerator.h"

#include "UsbPort.h"
#include "UsbDevicesManager.h"
#include "ModuleManagerUsb.h"
#include "UsbDeviceConfiguration.h"
#include "UsbDeviceSingle.h"
#include "TextSingleLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbDeviceEnumerator::UsbDeviceEnumerator () :
  m_device_setup (),
  m_state (USB_ENUMERATOR_IDLE),
  m_active_port (NULL),
  m_configuring_device (NULL),
  m_timer ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   setState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::setState (TypeUsbEnumeratorState new_state)
{
  ASSERT_TEST (new_state != m_state);

  m_state = new_state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   portConnected
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::portConnected (UsbPort * connected_port)
{
  if (m_state == USB_ENUMERATOR_IDLE)
  {
    ASSERT_TEST ((m_active_port == NULL) && (m_device_setup.getChannelState () == USB_CHANNEL_DETACHED));

    m_active_port = connected_port;
    m_active_port->portResetAction (true);
    m_timer.leaseFromNow (55);

    setState (USB_ENUMERATOR_RESET);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   tick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::tick ()
{
  switch (m_state)
  {
    case USB_ENUMERATOR_IDLE:
      break;

    case USB_ENUMERATOR_RESET:
      tickReset ();
      break;

    case USB_ENUMERATOR_WAIT_ENABLE:
      tickWaitEnable ();
      break;

    case USB_ENUMERATOR_ERROR:
      waitErrorState ();
      break;

    default:
      if (waitControlReply ())
      {
        parseDeviceReply ();
      }
      break;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   parseDeviceReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::parseDeviceReply ()
{
  switch (m_state)
  {
    case USB_ENUMERATOR_GET_MAXIMUM_PACKET_SIZE:
      parseGetMaximumPacketSize ();
      break;

    case USB_ENUMERATOR_SET_ADDRESS:
      setAddressAck ();
      break;

    case USB_ENUMERATOR_GET_DEVICE_DESCRIPTOR:
      parseDeviceDescriptor ();
      break;

    case USB_ENUMERATOR_GET_CONFIGURATION:
      parseConfiguration ();
      break;

    case USB_ENUMERATOR_DEVICE_CONFIGURATION:
      tickDeviceConfiguration ();
      break;

    default:
      ASSERT_CRITICAL (false);
      break;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   tickReset
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::tickReset ()
{
  switch (m_active_port->getPortState ())
  {
    case USB_PORT_RESET_ACTIVE:
      if (m_timer.testExpired ())
      {
        m_active_port->portResetAction (false);
        setState (USB_ENUMERATOR_WAIT_ENABLE);
      }
      break;

    default:
      setState (USB_ENUMERATOR_IDLE);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   tickWaitEnable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::tickWaitEnable ()
{
  switch (m_active_port->getPortState ())
  {
    case USB_PORT_ENABLED:
      {
        m_device_setup.attachChannel (m_active_port);

        // Request device descriptor.
        sendControlRequest (USB_DEVICE_REQUEST_GET_DESCRIPTOR, 0, 8, 100);
        setState (USB_ENUMERATOR_GET_MAXIMUM_PACKET_SIZE);
      }
      break;

    case USB_PORT_CONNECTED:
      // The port manager limits this state to 100 ms.
      break;

    case USB_PORT_POWER_DOWN_PENDING:
      registerControlError ("Port powering down");
      break;

    default:
      registerControlError ("Unexpected port state.");
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   parseGetMaximumPacketSize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::parseGetMaximumPacketSize ()
{
  const uint8_t * reply = m_device_setup.getControlReply ();
  ASSERT_TEST (reply != NULL);

  if (reply[0] < 18)
  {
    return registerControlError ("Device descriptor too short.");
  }

  if ((reply[1] != 0x01) || (reply[4] != 0) || (reply[7] < 8))
  {
    return registerControlError ("Device descriptor mismatch.");
  }

  m_device_setup.setupChannelParam (USB_CHANNEL_PARAM_MAXIMUM_PACKET_SIZE, reply[7]);

  ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
  UsbDevicesManager & devices_manager = module_usb.getDevicesManager ();
  uint16_t device_address = devices_manager.getFreeAddress ();

  // Set device new address.
  sendControlRequest (USB_DEVICE_REQUEST_SET_ADDRESS, device_address, 0, 100);
  setState (USB_ENUMERATOR_SET_ADDRESS);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   setAddressAck
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::setAddressAck ()
{
  // The usb 2.0 spec alows devices 2 ms to update their address.
  Common::waitMicro (2000);

  uint16_t device_address = m_device_setup.getValue ();
  m_device_setup.setupChannelParam (USB_CHANNEL_PARAM_DEVICE_ADDRESS, device_address);

  // Request device descriptor.
  sendControlRequest (USB_DEVICE_REQUEST_GET_DESCRIPTOR, 0, 18, 100);
  setState (USB_ENUMERATOR_GET_DEVICE_DESCRIPTOR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   waitErrorState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::waitErrorState ()
{
  TypeUsbChannelState control_state = m_device_setup.getChannelState ();
  switch (control_state)
  {
    case USB_CHANNEL_DETACHED:
      setState (USB_ENUMERATOR_IDLE);
      break;

    case USB_CHANNEL_ERROR:
      break;

    default:
      ASSERT_TEST (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   parseDeviceDescriptor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::parseDeviceDescriptor ()
{
  const uint8_t * reply = m_device_setup.getControlReply ();
  ASSERT_TEST (reply != NULL);

  if ((reply[0] < 18) || (reply[7] != m_device_setup.getChannelParam (USB_CHANNEL_PARAM_MAXIMUM_PACKET_SIZE)))
  {
    return registerControlError ("Device descriptor mismatch.");
  }

  if (reply[17] != 0x01)
  {
    return registerControlError ("Code supports only single configurations.");
  }

  // Get short configuration.
  sendControlRequest (USB_DEVICE_REQUEST_GET_CONFIGURATION, 0, 8, 100);
  setState (USB_ENUMERATOR_GET_CONFIGURATION);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   parseConfiguration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::parseConfiguration ()
{
  const uint8_t * reply = m_device_setup.getControlReply ();
  ASSERT_TEST (reply != NULL);

  uint16_t reply_size = m_device_setup.getReplySize ();
  uint16_t configuration_length = U8LE2U16 ((reply + 2));

  if (reply_size < configuration_length)
  {
    // Short configuration prefix.
    ASSERT_TEST (reply_size == 8);
    sendControlRequest (USB_DEVICE_REQUEST_GET_CONFIGURATION, 0, configuration_length, 100);
    // State remains 'USB_ENUMERATOR_GET_CONFIGURATION'.
  }
  else
  {
    // Complete configuration data.
    UsbDeviceConfiguration configuration (m_device_setup);

    setConfiguringDevice (configuration);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   setConfiguringDevice
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::setConfiguringDevice (const UsbDeviceConfiguration & configuration)
{
  ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
  UsbDevicesManager & devices_manager = module_usb.getDevicesManager ();
  m_configuring_device = devices_manager.getDeviceHandler (configuration);

  if (m_configuring_device == NULL)
  {
    return registerControlError ("No supporting device found.");
  }

  // Limit device configuration to 1 second.
  m_timer.leaseFromNow (1000);

  setState (USB_ENUMERATOR_DEVICE_CONFIGURATION);
  m_configuring_device->initiateConfiguration (m_device_setup);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   tickDeviceConfiguration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::tickDeviceConfiguration ()
{
  switch (m_configuring_device->getDeviceState ())
  {
    case USB_DEVICE_SINGLE_CONFIGURING:
      return m_configuring_device->registerSetupReply (m_device_setup);

    case USB_DEVICE_SINGLE_ACTIVE:
      return registerDeviceReady ();

    case USB_DEVICE_SINGLE_ERROR:
      return registerControlError ("Device configuration error.");

    case USB_DEVICE_SINGLE_CLOSED:
      return registerControlError ("Unexpected error.");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   sendControlRequest
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::sendControlRequest (TypeUsbDeviceRequest request, uint16_t value, uint16_t reply_size,
                                         uint32_t timeout)
{
  m_device_setup.controlTransaction (request, value, reply_size);

  m_timer.leaseFromNow (timeout);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   waitControlReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
UsbDeviceEnumerator::waitControlReply ()
{
  if (m_timer.testExpired ())
  {
    registerControlError ("Device reply timeout.");
    return false;
  }

  TypeUsbChannelState control_state = m_device_setup.getChannelState ();

  if (control_state == USB_CHANNEL_IDLE)
  {
    return true;
  }

  if ((control_state != USB_CHANNEL_ACTIVE) &&
      (control_state != USB_CHANNEL_READY_IN) && (control_state != USB_CHANNEL_READY_OUT))
  {
    registerControlError ("Channel error.");
    return false;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   registerDeviceReady
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::registerDeviceReady ()
{
  TextSingleLine ().print ("Device configured successfully.");

  ASSERT_TEST (m_configuring_device != NULL);

  m_configuring_device = NULL;
  m_active_port = NULL;
  m_device_setup.detachChannel ();

  setState (USB_ENUMERATOR_IDLE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceEnumerator
// @method:   registerControlError
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceEnumerator::registerControlError (const char * message)
{
  TextSingleLine ().print ("Aborting device configuration - ") << message;

  ASSERT_TEST (m_active_port != NULL);

  if (m_configuring_device != NULL)
  {
    m_configuring_device->configurationError ();
    m_configuring_device = NULL;
  }

  // Disabled port would cause the control channel to detach through the channels manager.
  m_active_port->disablePort ();
  m_active_port = NULL;

  setState (USB_ENUMERATOR_ERROR);
}

