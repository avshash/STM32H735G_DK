#include "Common.h"
#include "UsbChannelSingleControl.h"

#include "UsbPort.h"
#include "TextSingleLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControl
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbChannelSingleControl::UsbChannelSingleControl () :
  UsbChannelSingle ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControl
// @method:   nextPacketOut
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleControl::nextPacketOut (uint32_t * target, uint16_t bytes_count, uint16_t bytes_offset)
{
  ASSERT_TEST ((bytes_count == 8) && (bytes_offset == 0));

  const uint32_t * request = getRequest ();
  target[0] = request[0];
  target[1] = request[1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControl
// @method:   nextPacketIn
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleControl::nextPacketIn (uint32_t const * source, uint16_t bytes_count, uint16_t bytes_offset)
{
  ASSERT_TEST ((bytes_offset % 4) == 0);
  ASSERT_TEST ((bytes_count + bytes_offset) <= sizeof (m_buffer));

  uint16_t words_count = (bytes_count / 4) + ((bytes_count & 0x03) ? 1 : 0);

  const uint32_t * source_limit = source + words_count;
  uint32_t * target = m_buffer + (bytes_offset / 4);

  while (source < source_limit)
  {
    *target = *source;
    target++;
    source++;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControl
// @method:   notifyTransactionComplete
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleControl::notifyTransactionComplete (TypeUsbChannelState channel_state)
{
  switch (channel_state)
  {
    case USB_CHANNEL_READY_IN:
      printMessage ((const uint8_t *) m_buffer, true);
      break;

    case USB_CHANNEL_READY_OUT:
      printMessage ((const uint8_t  *) getRequest (), false);
      receiveReply (getReplySize ());
      break;

    case USB_CHANNEL_NACK:
      channelError ();
      break;

    case USB_CHANNEL_ERROR:
    case USB_CHANNEL_DETACHED:
    case USB_CHANNEL_IDLE:
    case USB_CHANNEL_ACTIVE:
      ASSERT_TEMP (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControl
// @method:   getControlReply
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const uint8_t *
UsbChannelSingleControl::getControlReply () const
{
  if (getChannelState () == USB_CHANNEL_IDLE)
  {
    return (const uint8_t *) m_buffer;
  }

  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControl
// @method:   attachChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleControl::attachChannel (UsbPort * usb_port)
{
  ASSERT_TEST (getChannelState () == USB_CHANNEL_DETACHED);
  UsbChannelSingle::attachChannel (USB_CHANNEL_ENDPOINT_CONTROL, usb_port);
  ASSERT_TEST (getChannelState () == USB_CHANNEL_IDLE);

  setupChannelParam (USB_CHANNEL_PARAM_DEVICE_ADDRESS, 0);
  setupChannelParam (USB_CHANNEL_PARAM_MAXIMUM_PACKET_SIZE, 8);
  setupChannelParam (USB_CHANNEL_PARAM_EP_NUMBER, 0);

  bool b_low_speed = usb_port->testLowSpeed ();
  setupChannelParam (USB_CHANNEL_PARAM_DEVICE_IS_LOW_SPEED, b_low_speed ? 1 : 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControl
// @method:   printMessage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleControl::printMessage (uint8_t const * message, bool b_is_input)
{
  int message_size = b_is_input ? getReplySize () : 8;

  if (message_size == 0)
  { 
    TextSingleLine ().print ("Device answered 'ack'.");
  } 

  int offset = 0;
  while (offset < message_size)
  {
    TextSingleLine reply_print;
    uint16_t reminder = message_size - offset;
    uint16_t to_print_count = MIN (8, reminder);
  
    reply_print << TEXT_STYLE_HEX2;
    if (b_is_input)
    {
      reply_print << "Device reply offset 0x" << offset << " :";
    }
    else
    {
      reply_print << "Sending control request:";
    }

    for (uint16_t line_offset = 0; line_offset < to_print_count; line_offset++)
    {
      reply_print << " 0x" << message[offset + line_offset];
    } 

    offset = offset + to_print_count;
  }
}

