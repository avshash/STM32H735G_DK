#include "Common.h"
#include "UsbChannelSingleControlDevice.h"

#include <cstring>

namespace
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   UsbControlRequestMap
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UsbControlRequestMap
{
  TypeUsbDeviceRequest m_type;
  uint16_t m_request;
  uint16_t m_default_value;
};

UsbControlRequestMap g_request_map[] =
{
  {USB_DEVICE_REQUEST_NONE, 0x0000UL, 0x0000},
  {USB_DEVICE_REQUEST_GET_DESCRIPTOR, 0x0680, 0x0100},
  {USB_DEVICE_REQUEST_SET_ADDRESS, 0x0500, 0x0000},
  {USB_DEVICE_REQUEST_GET_CONFIGURATION, 0x0680, 0x0200},
  {USB_DEVICE_REQUEST_SET_CONFIGURATION, 0x0900, 0x0001},
  {USB_DEVICE_REQUEST_SET_HID_BOOT, 0x0B21, 0x0000},
  {USB_DEVICE_REQUEST_SET_IDLE, 0x0A21, 0x0000},
  {USB_DEVICE_REQUEST_HID_STATE, 0x01A1, 0x0100},
  {USB_DEVICE_REQUEST_UNKNOWN, 0xFFFFU, 0xFFFFU}
};

};    // namespace

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControlDevice
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbChannelSingleControlDevice::UsbChannelSingleControlDevice  () :
  UsbChannelSingleControl ()
{
  m_transaction[0] = 0;
  m_transaction[1] = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControlDevice
// @method:   attachChannel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleControlDevice::attachChannel (UsbPort * usb_port)
{
  m_transaction[0] = 0;
  m_transaction[1] = 0;

  UsbChannelSingleControl::attachChannel (usb_port);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControlDevice
// @method:   controlTransaction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbChannelSingleControlDevice::controlTransaction (TypeUsbDeviceRequest request_type,
                                                   uint16_t override_value, uint16_t reply_size)
{
  m_transaction[0] = 0xFFFFFFFFUL;

  for (int i = 0; g_request_map[i].m_type != USB_DEVICE_REQUEST_UNKNOWN; i++)
  {
    if (g_request_map[i].m_type == request_type)
    {
      ASSERT_TEST (i == g_request_map[i].m_type);
      ASSERT_TEST ((override_value == 0) || (g_request_map[i].m_default_value == 0));

      uint32_t actual_value = (override_value == 0) ? g_request_map[i].m_default_value : override_value;
      m_transaction[0] = g_request_map[i].m_request | (actual_value << 16);
      break;
    }
  }

  ASSERT_TEST (m_transaction[0] != 0xFFFFFFFFUL);
  m_transaction[1] = ((uint32_t) reply_size) << 16;

  sendRequest (8);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControlDevice
// @method:   getActiveRequest
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeUsbDeviceRequest
UsbChannelSingleControlDevice::getActiveRequest () const
{
  uint16_t request = m_transaction[0] & 0x0000FFFFUL;

  for (int i = 0; g_request_map[i].m_type != USB_DEVICE_REQUEST_UNKNOWN; i++)
  {
    if (g_request_map[i].m_request == request)
    {
      uint32_t default_value = g_request_map[i].m_default_value;
      if ((default_value == 0) || (default_value == (m_transaction[0] >> 16)))
      {
        return (TypeUsbDeviceRequest) i;
      }
    }
  }

  ASSERT_CRITICAL (request == g_request_map[USB_DEVICE_REQUEST_UNKNOWN].m_request);

  return USB_DEVICE_REQUEST_UNKNOWN;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControlDevice
// @method:   getValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const uint32_t *
UsbChannelSingleControlDevice::getRequest () const
{
  return m_transaction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControlDevice
// @method:   getValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t
UsbChannelSingleControlDevice::getValue () const
{
  return ((m_transaction[0] >> 16) & 0x0000FFFFUL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbChannelSingleControlDevice
// @method:   getReplySize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t
UsbChannelSingleControlDevice::getReplySize () const
{
  return ((m_transaction[1] >> 16) & 0x0000FFFFUL);
}


