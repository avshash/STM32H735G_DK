#ifndef _USB_CHANNEL_SINGLE_INTERRUPT_H_
#define _USB_CHANNEL_SINGLE_INTERRUPT_H_

#include "UsbChannelSingle.h"

class UsbEndpointParams;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbChannelSingleInterrupt : public UsbChannelSingle
{
public:
  UsbChannelSingleInterrupt ();

  void attachChannel (UsbChannelSingle & control, uint16_t interval, uint16_t report_size);
  void requestReport ();
  const uint8_t * getReport ();

private:
  // Following methods may be called inside an otg interrupt handler. Implement efficiently.
  virtual void nextPacketOut (uint32_t *, uint16_t, uint16_t) final;
  virtual void nextPacketIn (uint32_t const * source, uint16_t bytes_count, uint16_t bytes_offset) final;
  virtual void notifyTransactionComplete (TypeUsbChannelState) final;

  uint8_t m_report_size;
  uint8_t m_interval;
  uint32_t m_buffer[2];
  uint32_t m_active_report[2];
};

#endif  // _USB_CHANNEL_SINGLE_INTERRUPT_H_
