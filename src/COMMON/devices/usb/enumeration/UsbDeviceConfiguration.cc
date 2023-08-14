#include "Common.h"
#include "UsbDeviceConfiguration.h"

#include "UsbChannelSingleControl.h"
#include "TextSingleLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbDeviceConfiguration::UsbDeviceConfiguration  (const UsbChannelSingleControl & control_channel) :
  m_device_class (USB_DEVICE_CLASS_UNKNOWN),
  m_endpoints_descriptor ()
{
  const uint8_t * raw_data = control_channel.getControlReply ();
  uint16_t data_length = control_channel.getReplySize ();

  parseRawData (raw_data, data_length);
  validateParams ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   parseRawData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceConfiguration::parseRawData (uint8_t const * raw_data, uint16_t data_length)
{
  const uint8_t * data_limit = raw_data + data_length;

  while ((raw_data < data_limit) && (m_device_class != USB_DEVICE_CLASS_ERROR))
  {
    uint8_t descriptor_size = raw_data[0];
    if ((descriptor_size < 2) || (data_limit < (raw_data + descriptor_size)))
    {
      markError ("Misformed configuration packet.");
      break;
    }

    parseDescriptor (raw_data);

    raw_data = raw_data + descriptor_size;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   parseDescriptor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceConfiguration::parseDescriptor (uint8_t const * descriptor_base)
{
  switch (descriptor_base[1])
  {
    case 0x02:
      parseConfigurationDescriptor (descriptor_base);
      break;

    case 0x04:
      parseInterfaceDescriptor (descriptor_base);
      break;

    case 0x05:
      parseEndpointDescriptor (descriptor_base);
      break;

    default:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   parseConfigurationDescriptor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceConfiguration::parseConfigurationDescriptor (const uint8_t * descriptor_base)
{
  if (descriptor_base[0] != 9)
  {
    return markError ("Configuration packet data length mismatch.");
  }

  if (descriptor_base[4] != 0x01)
  {
    return markError ("Do not support multiple interfaces.");
  }

  if (descriptor_base[5] != 0x01)
  {
    return markError ("Do not support multiple configurations.");
  }

  if ((descriptor_base[7] & 0x80) == 0)
  {
    return markError ("Configuration descriptor error.");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   parseInterfaceDescriptor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceConfiguration::parseInterfaceDescriptor (const uint8_t * descriptor_base)
{
  if (descriptor_base[0] != 9)
  {
    return markError ("Interface packet data length mismatch.");
  }

  if ((descriptor_base[2] != 0x00) || (m_device_class != USB_DEVICE_CLASS_UNKNOWN))
  {
    return markError ("Do not support multiple interfaces.");
  }

  if ((descriptor_base[4] < 1) || (4 < descriptor_base[4]))
  {
    return markError ("Interface must support between 1 and 4 endpoints.");
  }

  switch (descriptor_base[5])
  {
    case 0x03:
      if (descriptor_base[6] != 0x01)
      {
        return markError ("Support only HID boot interfaces.");
      }

      if (descriptor_base[7] == 0x01)
      {
        m_device_class = USB_DEVICE_CLASS_HID_KEYBOARD;
        break;
      }

      if (descriptor_base[7] == 0x02)
      {
        m_device_class = USB_DEVICE_CLASS_HID_MOUSE;
        break;
      }

      break;

    case 0x07:
      m_device_class = USB_DEVICE_CLASS_PRINTER;
      break;

    default:
      break;
  }

  if (m_device_class == USB_DEVICE_CLASS_UNKNOWN)
  {
    return markError ("Unsupported device class.");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   parseEndpointDescriptor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceConfiguration::parseEndpointDescriptor (const uint8_t * descriptor_base)
{
  if (!m_endpoints_descriptor.parseDescriptor (descriptor_base))
  {
    markError ("Enpoint parsing failure.");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   validateParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceConfiguration::validateParams ()
{
  if (m_device_class == USB_DEVICE_CLASS_ERROR)
  {
    return;
  }

  if (m_device_class == USB_DEVICE_CLASS_UNKNOWN)
  {
    return markError ("Missing configuration descriptor.");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   markError
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceConfiguration::markError (const char * error_message)
{
  m_device_class = USB_DEVICE_CLASS_ERROR;

  TextSingleLine single_line;
  single_line << "Aborting configuration because " << error_message;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   getDeviceClass
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeUsbDeviceClass
UsbDeviceConfiguration::getDeviceClass () const
{
  return m_device_class;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
// @method:   getEndpointParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const UsbEndpointParams *
UsbDeviceConfiguration::getEndpointParams (TypeUsbChannelEndpoint type) const
{
  if (m_device_class == USB_DEVICE_CLASS_ERROR)
  {
    return NULL;
  }

  return m_endpoints_descriptor.getEndpointParams (type);
}

