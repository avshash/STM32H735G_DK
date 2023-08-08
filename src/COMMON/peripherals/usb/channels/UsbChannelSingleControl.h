#ifndef _USB_CHANNEL_SINGLE_CONTROL_H_
#define _USB_CHANNEL_SINGLE_CONTROL_H_

#include "UsbChannelSingle.h"
#include "UsbControlAction.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbChannelSingleControl : public UsbChannelSingle
{
public:
  const uint8_t * getControlReply () const;
  virtual uint16_t getReplySize () const = 0;

protected:
  UsbChannelSingleControl ();

  void attachChannel (UsbPort * usb_port);
  void printMessage (uint8_t const * message, bool b_is_input);

private:
  virtual const uint32_t * getRequest () const = 0;
  virtual void notifyTransactionComplete (TypeUsbChannelState) final;

  // Following methods may be called inside an otg interrupt handler. Implement efficiently.
  virtual void nextPacketOut (uint32_t * target, uint16_t bytes_count, uint16_t bytes_offset) final;
  virtual void nextPacketIn (uint32_t const * source, uint16_t bytes_count, uint16_t bytes_offset) final;

  uint32_t m_buffer[16];
};

#endif  // _USB_CHANNEL_SINGLE_CONTROL_H_
