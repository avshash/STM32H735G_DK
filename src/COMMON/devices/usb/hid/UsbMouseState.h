#ifndef _USB_MOUSE_STATE_H_
#define _USB_MOUSE_STATE_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   UsbMouseState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct UsbMouseState
{
  uint8_t m_mouse_is_active;
  uint8_t m_mouse_buttons;
  uint16_t m_x;
  uint16_t m_y;
};

#endif  // _USB_MOUSE_STATE_H_
