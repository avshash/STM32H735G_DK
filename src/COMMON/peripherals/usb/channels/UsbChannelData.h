#ifndef _USB_CHANNEL_DATA_H_
#define _USB_CHANNEL_DATA_H_

#include "TypeUsbChannelState.h"

class UsbChannelSingle;
class UsbPort;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   UsbChannelData
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UsbChannelData
{
  UsbChannelSingle * m_handler;
  UsbPort * m_port;
  volatile TypeUsbChannelState m_channel_state;
  uint16_t m_data_offset;
  uint16_t m_data_size;
  uint32_t m_action_timeout;
};

#endif  // _USB_CHANNEL_DATA_H_

