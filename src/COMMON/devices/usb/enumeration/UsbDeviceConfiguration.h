#ifndef _USB_DEVICE_CONFIGURATION_H_
#define _USB_DEVICE_CONFIGURATION_H_

#include "TypeUsbDeviceClass.h"
#include "UsbEndpointsDescriptor.h"
#include "TypeUsbChannelParam.h"
#include "TypeUsbChannelEndpoint.h"

class UsbChannelSingleControl;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceConfiguration
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbDeviceConfiguration
{
public:
  UsbDeviceConfiguration (const UsbChannelSingleControl &);

  TypeUsbDeviceClass getDeviceClass () const;
  const UsbEndpointParams * getEndpointParams (TypeUsbChannelEndpoint) const;

private:
  void parseRawData (uint8_t const * raw_data, uint16_t data_length);
  void parseDescriptor (const uint8_t * descriptor_base);
  void parseConfigurationDescriptor (const uint8_t * descriptor_base);
  void parseInterfaceDescriptor (const uint8_t * descriptor_base);
  void parseEndpointDescriptor (const uint8_t * descriptor_base);
  void validateParams ();
  void markError (const char * error_message);

  TypeUsbDeviceClass m_device_class;
  UsbEndpointsDescriptor m_endpoints_descriptor;
};

#endif  // _USB_DEVICE_CONFIGURATION_H_
