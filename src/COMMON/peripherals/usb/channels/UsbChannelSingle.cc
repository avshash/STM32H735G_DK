#include "Common.h"
#include "UsbChannelSingle.h"

#include "ModuleManagerUsb.h"
#include "UsbChannelsManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbChannelSingle::UsbChannelSingle () :
  m_channel_index (-1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   setupChannelParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingle::setupChannelParam (TypeUsbChannelParam param_type, uint16_t new_value)
{
  if (m_channel_index < 0)
  {
    ASSERT_TEST (false);
    return;
  }

  // Channels manager verifies that this channel is not currently enabled.
  ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
  UsbChannelsManager & channels_manager = module_usb.getChannelsManager ();

  channels_manager.setupChannelParam (m_channel_index, param_type, new_value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   getChannelParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t
UsbChannelSingle::getChannelParam (TypeUsbChannelParam param_type) const
{
  if (m_channel_index < 0)
  {
    ASSERT_TEST (false);
    return 0;
  }

  // Channels manager verifies that this channel is not currently enabled.
  ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
  UsbChannelsManager & channels_manager = module_usb.getChannelsManager ();

  return channels_manager.getChannelParam (m_channel_index, param_type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   attachChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingle::attachChannel (TypeUsbChannelEndpoint channel_type, UsbPort * usb_port)
{
  ASSERT_TEST (m_channel_index == -1);

  ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
  UsbChannelsManager & channels_manager = module_usb.getChannelsManager ();

  m_channel_index = channels_manager.attachChannel (channel_type, this, usb_port);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   getUsbPort
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbPort *
UsbChannelSingle::getUsbPort ()
{
  ASSERT_TEST (0 <= m_channel_index);

  ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
  UsbChannelsManager & channels_manager = module_usb.getChannelsManager ();

  return channels_manager.getUsbPort (m_channel_index);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   channelError
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingle::channelError ()
{
  if (0 <= m_channel_index)
  {
    m_channel_index = -2;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   detachChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingle::detachChannel ()
{
  if (0 <= m_channel_index)
  {
    ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
    UsbChannelsManager & channels_manager = module_usb.getChannelsManager ();
    channels_manager.detachChannel (m_channel_index);

    m_channel_index = -1;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   getChannelState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeUsbChannelState
UsbChannelSingle::getChannelState () const
{
  if (0 <= m_channel_index)
  {
    ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
    UsbChannelsManager & channels_manager = module_usb.getChannelsManager ();

    return channels_manager.getChannelState (m_channel_index);
  }

  if (m_channel_index == -1)
  {
    return USB_CHANNEL_DETACHED;
  }

  return USB_CHANNEL_ERROR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   sendRequest
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingle::sendRequest (uint16_t request_length, uint8_t enable_delay)
{
  ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
  UsbChannelsManager & channels_manager = module_usb.getChannelsManager ();

  channels_manager.sendRequest (m_channel_index, request_length, enable_delay);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   receiveReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingle::receiveReply (uint16_t request_length, uint8_t enable_delay)
{
  ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
  UsbChannelsManager & channels_manager = module_usb.getChannelsManager ();

  channels_manager.receiveReply (m_channel_index, request_length, enable_delay);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingle
// @method:   tick
// @return:   'false' when channel is detached (must be an error state). 'true' if all is good.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
UsbChannelSingle::tick (TypeUsbChannelState channel_state)
{
  if (m_channel_index < 0)
  {
    ASSERT_TEST (m_channel_index < -1);

    // If we have any error state, move to detached.
    m_channel_index = -1;
    return false;
  }

  if ((channel_state == USB_CHANNEL_ERROR) || (channel_state == USB_CHANNEL_DETACHED))
  {
    ASSERT_TEST (channel_state != USB_CHANNEL_DETACHED);
    m_channel_index = -1;
    return false;
  }

  return true;
}

