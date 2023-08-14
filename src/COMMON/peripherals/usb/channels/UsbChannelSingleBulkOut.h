#ifndef _USB_CHANNEL_SINGLE_BULK_OUT_H_
#define _USB_CHANNEL_SINGLE_BULK_OUT_H_

#include "UsbChannelSingle.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleBulkOut
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbChannelSingleBulkOut : public UsbChannelSingle
{
public:
  UsbChannelSingleBulkOut ();

  void attachChannel (UsbChannelSingle & control);
  void initTransaction (const uint32_t * buffer, uint16_t bytes_count);

private:
  // Following methods may be called inside an otg interrupt handler. Implement efficiently.
  virtual void nextPacketOut (uint32_t * target, uint16_t bytes_count, uint16_t bytes_offset) final;
  virtual void nextPacketIn (uint32_t const *, uint16_t, uint16_t) final;
  virtual void notifyTransactionComplete (TypeUsbChannelState) final;

  uint32_t const * m_buffer;
};

#endif  // _USB_CHANNEL_SINGLE_BULK_OUT_H_
