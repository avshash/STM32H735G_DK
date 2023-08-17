#include "Common.h"
#include "UsbChannelSingleBulkOut.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleBulkOut
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbChannelSingleBulkOut::UsbChannelSingleBulkOut () :
  UsbChannelSingle (),
  m_buffer (NULL)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleBulkOut
// @method:   attachChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleBulkOut::attachChannel (UsbChannelSingle & control)
{
  UsbChannelSingle::attachChannel (USB_CHANNEL_ENDPOINT_BULK_OUT, control.getUsbPort ());

  uint16_t device_address = control.getChannelParam (USB_CHANNEL_PARAM_DEVICE_ADDRESS);
  setupChannelParam (USB_CHANNEL_PARAM_DEVICE_ADDRESS, device_address);

  uint16_t b_device_is_low_speed = control.getChannelParam (USB_CHANNEL_PARAM_DEVICE_IS_LOW_SPEED);
  setupChannelParam (USB_CHANNEL_PARAM_DEVICE_IS_LOW_SPEED, b_device_is_low_speed);

  m_buffer = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleBulkOut
// @method:   nextPacketOut
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleBulkOut::nextPacketOut (uint32_t * target, uint16_t bytes_count, uint16_t bytes_offset)
{
  ASSERT_TEST (m_buffer != NULL);

  if ((bytes_offset % 4) != 0)
  {
    ASSERT_TEST (0);
    return;
  }

  int words_count = (bytes_count + 3) / 4;
  uint32_t const * source_base = m_buffer + (bytes_offset / 4);
  for (int offset = 0; offset < words_count; offset++)
  {
    target[offset] = source_base[offset];
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleBulkOut
// @method:   nextPacketIn
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleBulkOut::nextPacketIn (uint32_t const * source, uint16_t bytes_count, uint16_t bytes_offset)
{
  ASSERT_CRITICAL (false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleBulkOut
// @method:   notifyTransactionComplete
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleBulkOut::notifyTransactionComplete (TypeUsbChannelState reason)
{
  ASSERT_TEST (m_buffer != NULL);

  m_buffer = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleBulkOut
// @method:   initTransaction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleBulkOut::initTransaction (const uint32_t * buffer, uint16_t bytes_count)
{
  if (m_buffer != NULL)
  {
    ASSERT_TEST (false);
    return;
  }

  m_buffer = buffer;

  sendRequest (bytes_count);
}
