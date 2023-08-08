#include "Common.h"
#include "UsbChannelSingleInterrupt.h"

#include "TextSingleLine.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleInterrupt
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbChannelSingleInterrupt::UsbChannelSingleInterrupt () :
  UsbChannelSingle (),
  m_report_size (0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleInterrupt
// @method:   attachChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleInterrupt::attachChannel (UsbChannelSingle & control, uint16_t interval, uint16_t report_size)
{
  ASSERT_TEST ((m_report_size == 0) || (m_report_size == report_size));

  if (0 == interval)
  {
    ASSERT_TEST (false);
    return;
  }

  memset (m_buffer, 0, sizeof (m_buffer));
  memset (m_active_report, 0, sizeof (m_active_report));

  UsbChannelSingle::attachChannel (USB_CHANNEL_ENDPOINT_INTERRUPT_IN, control.getUsbPort ());

  uint16_t device_address = control.getChannelParam (USB_CHANNEL_PARAM_DEVICE_ADDRESS);
  setupChannelParam (USB_CHANNEL_PARAM_DEVICE_ADDRESS, device_address);

  uint16_t b_device_is_low_speed = control.getChannelParam (USB_CHANNEL_PARAM_DEVICE_IS_LOW_SPEED);
  setupChannelParam (USB_CHANNEL_PARAM_DEVICE_IS_LOW_SPEED, b_device_is_low_speed);

  TextSingleLine ().print ("Attach channel interrupts with interval of ") << interval <<
                           " and report size of " << report_size << ".";
  m_interval = interval;
  m_report_size = report_size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleInterrupt
// @method:   nextPacketOut
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleInterrupt::nextPacketOut (uint32_t *, uint16_t, uint16_t)
{
  ASSERT_CRITICAL (false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleInterrupt
// @method:   nextPacketIn
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleInterrupt::nextPacketIn (uint32_t const * source, uint16_t bytes_count, uint16_t bytes_offset)
{
  ASSERT_TEST ((bytes_count == m_report_size) && (bytes_offset == 0));

  m_buffer[0] = source[0];
  if (4 < bytes_count)
  {
    ASSERT_TEST (bytes_count <= 8);

    m_buffer[1] = source[1];
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleInterrupt
// @method:   requestReport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleInterrupt::requestReport ()
{
  receiveReply (m_report_size, m_interval);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleInterrupt
// @method:   getReport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const uint8_t *
UsbChannelSingleInterrupt::getReport ()
{
  return (const uint8_t *) m_active_report;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleInterrupt
// @method:   notifyTransactionComplete
// @note:     In case of a 'nack' answer, we can initiate another query.
// @note:     When we receive a reply, stop queries until owener gets notification.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleInterrupt::notifyTransactionComplete (TypeUsbChannelState reason)
{
  switch (reason)
  {
    case USB_CHANNEL_READY_IN:
      m_active_report[0] = m_buffer[0];
      m_active_report[1] = m_buffer[1];
      break;

    case USB_CHANNEL_NACK:
      receiveReply (m_report_size, m_interval);
      break;

    case USB_CHANNEL_ERROR:
      break;

    default:
      ASSERT_TEST (false);
      channelError ();
      break;
  }
}

