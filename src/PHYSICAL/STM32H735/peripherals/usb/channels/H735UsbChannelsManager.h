#ifndef _H735_USB_CHANNELS_MANAGER_H_
#define _H735_USB_CHANNELS_MANAGER_H_

#include "UsbChannelsManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @dscr:     Behavior is documented by PM0468 Pragramming manual, section 62.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735UsbChannelsManager : public UsbChannelsManager
{
public:
  H735UsbChannelsManager ();

private:
  virtual void initChannelTransaction (int channel_index, uint16_t data_size,
                                       uint8_t enable_delay, bool b_direction_in) final;
  virtual void setupChannel (int channel_index, TypeUsbChannelEndpoint) final;
  void setupHctSiz (int channel_index, uint32_t data_size, bool b_direction_in);
  void transactionCompleted (int channel_index, TypeUsbChannelState new_channel_state);

  // Following methods are never called inside an otg interrupt handler.
  virtual void setupChannelParam (int channel_index, TypeUsbChannelParam, uint16_t new_value) final;
  virtual uint16_t getChannelParam (int channel_index, TypeUsbChannelParam) const final;
  virtual void disableChannel (int channel_index) final;

  // Following methods are called inside an otg interrupt handler. Implement efficiently.
  virtual void markSof () final;
  virtual void handleRxPacket () final;
  void handleChannelOutInterrupt (int channel_index, uint32_t active_interrupts);
  void handleChannelInInterrupt (int channel_index, uint32_t active_interrupts);
  void channelSof (int channel_index);

  uint8_t m_channel_enable[15];
};

typedef H735UsbChannelsManager PhysicalUsbChannelsManager;

#endif  // _H735_USB_CHANNELS_MANAGER_H_

