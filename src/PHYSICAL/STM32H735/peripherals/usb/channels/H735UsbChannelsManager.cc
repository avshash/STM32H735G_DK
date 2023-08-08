#include "Common.h"
#include "H735UsbChannelsManager.h"

#include "UsbChannelSingle.h"
#include "PhysicalOtgFsRegisters.h"

#include <cstring>

namespace
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   UsbHccharStructure
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UsbHccharStructure
{
  TypeUsbChannelParam m_type;
  uint32_t m_param_mask;
  uint8_t m_param_shift;
};

UsbHccharStructure g_hcchar_structure[] =
{
  {USB_CHANNEL_PARAM_MAXIMUM_PACKET_SIZE, 0x000007FFUL, 0},
  {USB_CHANNEL_PARAM_DEVICE_ADDRESS, 0x1FC00000UL, 22},
  {USB_CHANNEL_PARAM_EP_NUMBER, 0x00007800UL, 11},
  {USB_CHANNEL_PARAM_DEVICE_IS_LOW_SPEED, 0x00020000UL, 17}
};

};    // namespace

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735UsbChannelsManager::H735UsbChannelsManager ()
{
  memset (m_channel_enable, 0, sizeof (m_channel_enable));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   markSof
// @dscr:     Tests each channel for pending actions - either error, message to send, or message to receive.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::markSof ()
{
  for (int channel_index = 0; channel_index < 16; channel_index++)
  {
    PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

    // Channel interrupts are masked. All actions are done during the 'start of frame' phase.
    if (channel_registers.HCINTMSK != 0)
    {
      channelSof (channel_index);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   disableChannel
// @note:     Never invoked while handling interrupts. Must be called while interrupts are enabled.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::disableChannel (int channel_index)
{
  PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

  // Mark channel as disabled for interrupts.
  m_channel_enable[channel_index] = 0;
  channel_registers.HCINTMSK = 0;

  // Disable channel only if enabled.
  if ((channel_registers.HCCHAR & 0x80000000UL) != 0)
  {
    // Both tx queues should be at least half empty. Saves the bother of which queue we need to address.
    while ((OTG_FS_REGISTERS.GINTSTS & 0x04000020UL) != 0x04000020UL);

    // Request channel halt.
    channel_registers.HCCHAR = channel_registers.HCCHAR | 0xC0000000UL;

    while ((channel_registers.HCINT & 0x00000003UL) == 0)
    {
      if (((OTG_FS_REGISTERS.GAHBCFG & 0x00000001UL) == 0) && ((OTG_FS_REGISTERS.GINTSTS & 0x00000010UL) != 0))
      {
        // When interrupts are disabled, must read incoming packets to mark channel as disabled.
        handleRxPacket ();
      }
    }

    ASSERT_TEMP ((channel_registers.HCCHAR & 0x80000000UL) == 0);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   getChannelParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::setupChannelParam (int channel_index, TypeUsbChannelParam param, uint16_t new_value)
{
  ASSERT_TEMP (0 <= channel_index);
  PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

  ASSERT_TEST (g_hcchar_structure[param].m_type == param);

  uint32_t hcchar_base = channel_registers.HCCHAR & (~g_hcchar_structure[param].m_param_mask);

  int param_shift = g_hcchar_structure[param].m_param_shift;

  ASSERT_TEST (new_value <= (g_hcchar_structure[param].m_param_mask >> param_shift));

  channel_registers.HCCHAR = hcchar_base | (((uint32_t) new_value) << param_shift);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   getChannelParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t
H735UsbChannelsManager::getChannelParam (int channel_index, TypeUsbChannelParam param) const
{
  ASSERT_TEMP (0 <= channel_index);
  PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

  ASSERT_TEST (g_hcchar_structure[param].m_type == param);

  uint32_t param_value = channel_registers.HCCHAR & g_hcchar_structure[param].m_param_mask;
  return (param_value >> g_hcchar_structure[param].m_param_shift);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   initChannelTransaction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::initChannelTransaction (int channel_index, uint16_t data_size,
                                                uint8_t enable_delay, bool b_direction_in)
{
  ASSERT_TEST ((OTG_FS_REGISTERS.GAHBCFG & 0x00000001UL) != 0);

  PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

  setupHctSiz (channel_index, data_size, b_direction_in);

  // Setup the interrupts mask.
  ASSERT_TEST ((channel_registers.HCINTMSK == 0) && (m_channel_enable[channel_index] == 0));
  
  // Clear past active interrupts.
  channel_registers.HCINT = 0x000007FFUL;

  if (b_direction_in)
  {
    channel_registers.HCCHAR = (channel_registers.HCCHAR & 0x3FFF7FFFUL) | 0x00008000UL;
    // Ignore the 'ack' interrupt for input channels.
    channel_registers.HCINTMSK = 0x000007DFUL;
  }
  else
  {
    channel_registers.HCCHAR = channel_registers.HCCHAR & 0x3FFF7FFFUL;
    channel_registers.HCINTMSK = 0x000007FFUL;
  }

  m_channel_enable[channel_index] = enable_delay;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   setupChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::setupChannel (int channel_index, TypeUsbChannelEndpoint channel_type)
{
  PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

  ASSERT_TEST ((channel_registers.HCCHAR & 0x80000000UL) == 0);
  ASSERT_TEST (channel_registers.HCINTMSK == 0);

  uint32_t hcchar_base = (channel_registers.HCCHAR & 0x20010000UL) | 0x00100000UL;
  switch (channel_type)
  {
    case USB_CHANNEL_ENDPOINT_CONTROL:
      channel_registers.HCCHAR = hcchar_base;
      break;

    case USB_CHANNEL_ENDPOINT_INTERRUPT_IN:
      channel_registers.HCCHAR = hcchar_base | 0x000C0000UL;
      break;

    case USB_CHANNEL_ENDPOINT_BULK_IN:
    case USB_CHANNEL_ENDPOINT_BULK_OUT:
      channel_registers.HCCHAR = hcchar_base | 0x00080000UL;
      break;
  }

  // Default data pid is DATA0.
  channel_registers.HCTSIZ = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   setupHctSiz
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::setupHctSiz (int channel_index, uint32_t data_size, bool b_direction_in)
{
  PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

  uint32_t hcchar_register = channel_registers.HCCHAR;
  uint16_t packet_size = hcchar_register & 0x000007FFUL;

  ASSERT_TEST ((hcchar_register & 0x80000000UL) == 0);
  ASSERT_TEST (1 <= packet_size);

  uint32_t packets_count = (data_size + packet_size - 1) / packet_size;
  // Even when we have no data, we still have the initial packet to send / receive.
  packets_count = MAX (1, packets_count);

  if (b_direction_in)
  {
    // When direction is IN, the last packet might be padded.
//    data_size = packets_count * packet_size;
  }
  ASSERT_CRITICAL (data_size < 0x00080000UL);

  uint32_t dpid_value = channel_registers.HCTSIZ & 0x60000000UL;
  if ((hcchar_register & 0x000C0000UL) == 0)
  {
    // Control channel special first packet marking. DATA1 on input, SETUP on output.
    dpid_value = b_direction_in ? 0x40000000UL : 0x60000000UL;
  }

  channel_registers.HCTSIZ = dpid_value | (packets_count << 19) | data_size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   handleChannelOutInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::handleChannelOutInterrupt (int channel_index, uint32_t active_interrupts)
{
  // Test for 'ack'.
  if ((active_interrupts & 0x00000021UL) != 0)
  {
    PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

    ASSERT_TEST ((active_interrupts & 0x00000021UL) == active_interrupts);

    // Test for 'xfrc' (transaction complete).
    if ((active_interrupts & 0x00000001UL) != 0)
    {
      return transactionCompleted (channel_index, USB_CHANNEL_READY_OUT);
    }

    channel_registers.HCINT = 0x00000020UL;

    return sendNextPacket (channel_index, OTG_FS_CHANNEL_BUFFER (channel_index));
  }

  transactionCompleted (channel_index, USB_CHANNEL_ERROR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   handleChannelInInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::handleChannelInInterrupt (int channel_index, uint32_t active_interrupts)
{
  if ((active_interrupts & 0x00000001UL) != 0)
  {
    return transactionCompleted (channel_index, USB_CHANNEL_READY_IN);
  }

  // Test for 'nack'
  if (active_interrupts == 0x00000010UL)
  {
    return transactionCompleted (channel_index, USB_CHANNEL_NACK);
  }

  // If we reached this point, we have an active error.
  transactionCompleted (channel_index, USB_CHANNEL_ERROR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   handleRxPacket
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::handleRxPacket ()
{
  uint32_t grxstsp = OTG_FS_REGISTERS.GRXSTSP;
  if ((grxstsp & 0x001E0000UL) != 0x00040000UL)
  {
    return;
  }

  int channel_index = (grxstsp & 0x0FUL);
  uint16_t packet_size = (grxstsp & 0x7FF0UL) >> 4;

  PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);
  if ((channel_registers.HCTSIZ & 0x1FF80000UL) != 0)
  {
    // This is not the last packet for this transaction. Request next packet on start of frame.
    m_channel_enable[channel_index] = 1;
  }

  receiveNextPacket (channel_index, OTG_FS_CHANNEL_BUFFER (channel_index), packet_size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   channelSof
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::channelSof (int channel_index)
{
  PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

  uint32_t active_interrupts = channel_registers.HCINTMSK & channel_registers.HCINT;
  bool b_channel_is_input = ((channel_registers.HCCHAR & 0x00008000UL) != 0);

  if (0 < m_channel_enable[channel_index])
  {
    m_channel_enable[channel_index]--;

    if (m_channel_enable[channel_index] == 0)
    {
      channel_registers.HCCHAR = channel_registers.HCCHAR | 0x80000000UL;

      // Tell 'handleChannelOutInterrupt' to send first packet.
      if (!b_channel_is_input)
      {
        handleChannelOutInterrupt (channel_index, 0x00000020UL);
      }
    }
  }

  if (active_interrupts != 0)
  {
    if (b_channel_is_input)
    {
      handleChannelInInterrupt (channel_index, active_interrupts);
    }
    else
    {
      handleChannelOutInterrupt (channel_index, active_interrupts);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbChannelsManager
// @method:   transactionCompleted
// @dscr:     Disables channel interrupts, and commits the state change (either good, or error).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbChannelsManager::transactionCompleted (int channel_index, TypeUsbChannelState new_channel_state)
{
  PhysicalOtgFsChannelRegisters & channel_registers = OTG_FS_CHANNEL_REGISTERS (channel_index);

  m_channel_enable[channel_index] = 0;
  channel_registers.HCINTMSK = 0;

  setChannelState (channel_index, new_channel_state);
}

