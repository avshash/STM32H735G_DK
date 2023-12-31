#ifndef _USB_CONTROL_ACTION_H_
#define _USB_CONTROL_ACTION_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   UsbControlAction
// @dscr:     Follows the usb specification 2.0 (27/4/2000), section 9.3, table 9-2.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UsbControlAction
{
  uint16_t m_request;
  uint16_t m_value;
  uint16_t m_index;
  uint16_t m_length;
};

#endif  // _USB_CONTROL_ACTION_H_

