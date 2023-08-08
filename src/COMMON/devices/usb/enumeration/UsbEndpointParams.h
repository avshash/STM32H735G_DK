#ifndef _USB_ENDPOINT_PARAMS_H_
#define _USB_ENDPOINT_PARAMS_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbEndpointParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbEndpointParams
{
public:
  UsbEndpointParams ();

  void parseDescriptor (const uint8_t * descriptor_base);

  uint8_t getEpNumber () const;
  uint16_t getMaximumPacketSize () const;
  uint8_t getInterval () const;

private:
  uint8_t m_ep_number;
  uint16_t m_maximum_packet_size;
  uint8_t m_interval;
};

#endif  // _USB_ENDPOINT_PARAMS_H_
