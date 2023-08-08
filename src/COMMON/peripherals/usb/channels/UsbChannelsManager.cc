#include "Common.h"
#include "UsbChannelsManager.h"

#include "UsbChannelSingle.h"
#include "TextSingleLine.h"
#include "UsbPort.h"
#include "SystemTime.h"
#include <cstring>

#define CHANNELS_COUNT ((int) (sizeof (m_active_channels) / sizeof (UsbChannelData)))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbChannelsManager::UsbChannelsManager ()
{
  ASSERT_DEV (USB_CHANNEL_DETACHED == 0);

  memset (m_active_channels, 0, sizeof (m_active_channels));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   tick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::tick ()
{
  for (int i = 0; i < CHANNELS_COUNT; i++)
  {
    if (m_active_channels[i].m_channel_state != USB_CHANNEL_DETACHED)
    {
      tickChannel (i);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   tickChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::tickChannel (int channel_index)
{
  // When port is closed, or when we have a transaction timeout, move to error state.
  if (m_active_channels[channel_index].m_port->getPortState () != USB_PORT_ENABLED)
  {
    setChannelError (channel_index, "port disabled.");
  }
 
  TypeUsbChannelState channel_state = testTransactionResult (channel_index);

  if ((channel_state == USB_CHANNEL_ACTIVE) &&
      (m_active_channels[channel_index].m_action_timeout < SystemTime::getSystemMilliTime ()))
  {
    setChannelError (channel_index, "transaction timeout.");
    channel_state = USB_CHANNEL_ERROR;
  }

  // Call channel tick. Detach channel if handler is closed.
  UsbChannelSingle * channel_handler = m_active_channels[channel_index].m_handler;
  if (!channel_handler->tick (channel_state))
  {
    setChannelError (channel_index, "deviced detached.");

    detachChannel (channel_index);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   getUsbPort
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbPort *
UsbChannelsManager::getUsbPort (int control_channel_index)
{
  ASSERT_TEST (0 <= control_channel_index);
  UsbPort * usb_port = m_active_channels[control_channel_index].m_port;
  ASSERT_TEST (usb_port != NULL);

  return usb_port;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   attachChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
UsbChannelsManager::attachChannel (TypeUsbChannelEndpoint channel_type, UsbChannelSingle * usb_channel, UsbPort * port)
{
  for (int index = 0; ; index++)
  {
    ASSERT_CRITICAL (index < CHANNELS_COUNT);

    if (m_active_channels[index].m_handler == NULL)
    {
      m_active_channels[index].m_handler = usb_channel;
      m_active_channels[index].m_port = port;
      setChannelState (index, USB_CHANNEL_IDLE);

      setupChannel (index, channel_type);

      return index;
    }
  }

  // Unreachable.
  return -2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   detachChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::detachChannel (int channel_index)
{
  disableChannel (channel_index);

  if (m_active_channels[channel_index].m_channel_state != USB_CHANNEL_IDLE)
  {
    ASSERT_TEST (m_active_channels[channel_index].m_channel_state == USB_CHANNEL_ERROR);

    if (m_active_channels[channel_index].m_port->getPortState () == USB_PORT_ENABLED)
    {
      m_active_channels[channel_index].m_port->disablePort ();
    }
  }

  m_active_channels[channel_index].m_handler = NULL;
  m_active_channels[channel_index].m_port = NULL;

  setChannelState (channel_index, USB_CHANNEL_DETACHED);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   sendRequest
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::sendRequest (int channel_index, uint16_t request_length, uint8_t enable_delay)
{
  setupChannelTransaction (channel_index, request_length, enable_delay, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   receiveReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::receiveReply (int channel_index, uint16_t reply_length, uint8_t enable_delay)
{
  setupChannelTransaction (channel_index, reply_length, enable_delay, true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   getChannelState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeUsbChannelState
UsbChannelsManager::getChannelState (int channel_index) const
{
  ASSERT_TEST (0 <= channel_index);

  return m_active_channels[channel_index].m_channel_state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   setupChannelTransaction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::setupChannelTransaction (int channel_index, uint16_t data_size,
                                             uint8_t enable_delay, bool b_direction_in)
{
  ASSERT_TEST ((m_active_channels[channel_index].m_channel_state == USB_CHANNEL_IDLE) &&
               (m_active_channels[channel_index].m_handler != NULL));

  m_active_channels[channel_index].m_data_offset = 0;
  m_active_channels[channel_index].m_data_size = data_size;
  setChannelState (channel_index, USB_CHANNEL_ACTIVE);

  initChannelTransaction (channel_index, data_size, enable_delay, b_direction_in);

  // Note that we send / receive exactly one packet each active cycle.
  // Set the timeout only after initiating the transaction.
  uint16_t packets_count = data_size / getChannelParam (channel_index, USB_CHANNEL_PARAM_MAXIMUM_PACKET_SIZE);
  uint32_t action_timeout = SystemTime::getSystemMilliTime () + packets_count + enable_delay + 10;
  m_active_channels[channel_index].m_action_timeout = action_timeout;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   setChannelError
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::setChannelError (int channel_index, const char * error_message)
{
  ASSERT_TEST (m_active_channels[channel_index].m_channel_state != USB_CHANNEL_DETACHED);

  if (m_active_channels[channel_index].m_channel_state != USB_CHANNEL_ERROR)
  {
    TextSingleLine ().print (error_message) << error_message;

    setChannelState (channel_index, USB_CHANNEL_ERROR);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   testTransactionResult
// @dscr:     Channel handler is notified exactly once that transaction completed.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeUsbChannelState
UsbChannelsManager::testTransactionResult (int channel_index)
{
  TypeUsbChannelState ret_val = m_active_channels[channel_index].m_channel_state;

  switch (ret_val)
  {
    case USB_CHANNEL_READY_IN:
    case USB_CHANNEL_READY_OUT:
      ASSERT_TEST (m_active_channels[channel_index].m_data_offset <= m_active_channels[channel_index].m_data_size);

      if (m_active_channels[channel_index].m_data_offset != m_active_channels[channel_index].m_data_size)
      {
        setChannelError (channel_index, "device error - reply too short.");
        return USB_CHANNEL_ERROR;
      }

      setChannelState (channel_index, USB_CHANNEL_IDLE);
      m_active_channels[channel_index].m_handler->notifyTransactionComplete (ret_val);
      break;

    case USB_CHANNEL_NACK:
      if (m_active_channels[channel_index].m_data_offset != 0)
      {
        setChannelError (channel_index, "device returned an unexpected nack.");
        return USB_CHANNEL_ERROR;
      }

      disableChannel (channel_index);
      setChannelState (channel_index, USB_CHANNEL_IDLE);
      m_active_channels[channel_index].m_handler->notifyTransactionComplete (USB_CHANNEL_NACK);
      break;

    default:
      break;
  }

  return ret_val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   setChannelState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::setChannelState (int channel_index, TypeUsbChannelState new_channel_state)
{
  if (new_channel_state == USB_CHANNEL_IDLE)
  {
    m_active_channels[channel_index].m_action_timeout = 0xFFFFFFFFUL;
  }

  m_active_channels[channel_index].m_channel_state = new_channel_state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   sendNextPacket
// @dscr:     Sends next out packet (if not at the end of transaction).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::sendNextPacket (int channel_index, uint32_t * target)
{
  uint16_t data_offset = m_active_channels[channel_index].m_data_offset; 
  uint16_t reminder = m_active_channels[channel_index].m_data_size - data_offset;

  if (reminder == 0)
  {
    return;
  }
  uint16_t max_packet_size = getChannelParam (channel_index, USB_CHANNEL_PARAM_MAXIMUM_PACKET_SIZE);
  uint16_t packet_size = MIN (max_packet_size, reminder);
  m_active_channels[channel_index].m_data_offset = data_offset + packet_size;

  UsbChannelSingle * channel_handler = m_active_channels[channel_index].m_handler;
  ASSERT_TEST ((channel_handler != NULL) && (m_active_channels[channel_index].m_channel_state == USB_CHANNEL_ACTIVE));

  channel_handler->nextPacketOut (target, packet_size, data_offset);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelsManager
// @method:   receiveNextPacket
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelsManager::receiveNextPacket (int channel_index, const uint32_t * source, uint16_t packet_size)
{
  uint16_t data_offset = m_active_channels[channel_index].m_data_offset;
  ASSERT_TEST (packet_size <= (m_active_channels[channel_index].m_data_size - data_offset));
  m_active_channels[channel_index].m_data_offset = data_offset + packet_size;

  UsbChannelSingle * channel_handler = m_active_channels[channel_index].m_handler;

  ASSERT_TEMP (channel_handler != NULL);

  channel_handler->nextPacketIn (source, packet_size, data_offset);
}

