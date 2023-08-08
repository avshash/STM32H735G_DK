#ifndef _USB_ENDPOINTS_DESCRIPTOR_H_
#define _USB_ENDPOINTS_DESCRIPTOR_H_

#include "UsbEndpointParams.h"
#include "TypeUsbChannelEndpoint.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbEndpointsDescriptor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbEndpointsDescriptor
{
public:
  UsbEndpointsDescriptor ();

  bool parseDescriptor (const uint8_t * descriptor_base);
  const UsbEndpointParams * getEndpointParams (TypeUsbChannelEndpoint) const;

private:
  UsbEndpointParams m_endpoint_interrupt_in;
  UsbEndpointParams m_endpoint_bulk_in;
  UsbEndpointParams m_endpoint_bulk_out;
};

#endif  // _USB_ENDPOINTS_DESCRIPTOR_H_
