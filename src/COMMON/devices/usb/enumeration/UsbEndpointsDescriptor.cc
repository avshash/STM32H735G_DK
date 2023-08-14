#include "Common.h"
#include "UsbEndpointsDescriptor.h"

#include "TextSingleLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbEndpointsDescriptor
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbEndpointsDescriptor::UsbEndpointsDescriptor  () :
  m_endpoint_interrupt_in (),
  m_endpoint_bulk_in (),
  m_endpoint_bulk_out ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbEndpointsDescriptor
// @method:   parseDescriptor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
UsbEndpointsDescriptor::parseDescriptor (const uint8_t * descriptor_base)
{
  UsbEndpointParams * params = NULL;
  bool b_is_input = ((descriptor_base[2] & 0x80) != 0);

  switch (descriptor_base[3] & 0x03)
  {
    case 0x00:
    case 0x01:
      break;

    case 0x02:
      params = b_is_input ? &m_endpoint_bulk_in : &m_endpoint_bulk_out;
      break;

    case 0x03:
      params = b_is_input ? &m_endpoint_interrupt_in : NULL;
      break;
  }

  if (params == NULL)
  {
    TextSingleLine ().print ("Unsupported endpoint type.");
    return false;
  }

  if (params->getEpNumber () != 0)
  {
    TextSingleLine ().print ("Do not support multiple endpoints of the same type.");
    return false;
  }

  params->parseDescriptor (descriptor_base);

  if (params->getEpNumber () == 0)
  {
    TextSingleLine ().print ("Enpoint parsing failure.");
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbEndpointsDescriptor
// @method:   getEndpointParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const UsbEndpointParams *
UsbEndpointsDescriptor::getEndpointParams (TypeUsbChannelEndpoint type) const
{
  UsbEndpointParams const * reply = NULL;

  switch (type)
  {
    case USB_CHANNEL_ENDPOINT_CONTROL:
      break;

    case USB_CHANNEL_ENDPOINT_INTERRUPT_IN:
      reply = &m_endpoint_interrupt_in;
      break;

    case USB_CHANNEL_ENDPOINT_BULK_IN:
      reply = &m_endpoint_bulk_in;
      break;

    case USB_CHANNEL_ENDPOINT_BULK_OUT:
      reply = &m_endpoint_bulk_out;
      break;
  }

  if ((reply != NULL) && (reply->getEpNumber () == 0))
  {
    reply = NULL;
  }

  return reply;
}

