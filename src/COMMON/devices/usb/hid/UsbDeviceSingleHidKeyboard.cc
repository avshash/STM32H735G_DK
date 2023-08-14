#include "Common.h"
#include "UsbDeviceSingleHidKeyboard.h"

#include "TextSingleLine.h"
#include <cstring>

namespace
{
  char g_keyboard_table[] =
  {
    0, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', '0', '\n', 0, 0, ' ', ' ', '-', '=', '[',
    ']', '\\', '#', ';', '\'', '`', ',', '.', '/', 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, '/', '*', '-', '+', '\n', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '0', '.', 0, 0, '=', 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, ',', '=', 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, '(', ')', '{', '}', ' ', 0, 'A', 'B', 'C', 'D',
    'E', 'F', 0, '^', '%', '<', '>', '&', 0, '|', 0, ':', '#', ' ', '@', '!'
  };

  char g_keyboard_table_shift[] =
  {
    0, 0, 0, 0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@',
    '#', '$', '%', '^', '&', '*', '(', ')', '\n', 0, 0, ' ', ' ', '_', '+', '{',
    '}', '|', '~', ':', '"', '~', '<', '>', '?', 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, '/', '*', '-', '+', '\n', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '0', '.', 0, 0, '=', 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, ',', '=', 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, '(', ')', '{', '}', ' ', 0, 'A', 'B', 'C', 'D',
    'E', 'F', 0, '^', '%', '<', '>', '&', 0, '|', 0, ':', '#', ' ', '@', '!'
  };
} // anonymous namespace.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHidKeyboard
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbDeviceSingleHidKeyboard::UsbDeviceSingleHidKeyboard  () :
  UsbDeviceSingleHid ()
{
  ASSERT_DEV (sizeof (g_keyboard_table) == sizeof (g_keyboard_table_shift));

  m_cur_key = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHidKeyboard
// @method:   getDeviceClass
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeUsbDeviceClass
UsbDeviceSingleHidKeyboard::getDeviceClass () const
{
  return USB_DEVICE_CLASS_HID_KEYBOARD;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHidKeyboard
// @method:   getBootReportSize
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t
UsbDeviceSingleHidKeyboard::getBootReportSize () const
{
  return 8;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHidKeyboard
// @method:   registerReport
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSingleHidKeyboard::registerReport (const uint8_t * report)
{
  if (report[2] == 1)
  {
    m_cur_key = -1;
    return;
  }

  if (sizeof (g_keyboard_table) <= report[2])
  {
    TextSingleLine ().print ("Out of range character ") << (uint32_t) report[2];
    m_cur_key = -1;
    return;
  }

  if (report[2] == 0)
  {
    if ((0 < m_cur_key) && (m_cur_key <= 0x7F))
    {
      char key_print[2] = {m_cur_key, 0};
      TextSingleLine ().print ("Released key '") << key_print << "'.";
    }
    m_cur_key = 0;
  }
  else
  {
    if (report[3] != 0)
    {
      m_cur_key = -1;
      return;
    }

    const char * key_map = ((report[0] & 0x22) != 0) ? g_keyboard_table_shift : g_keyboard_table;
    char cur_key = key_map[report[2]];

    m_cur_key = cur_key;

    if ((m_cur_key == 0) && (report[2] != 0))
    {
      TextSingleLine ().print ("Unknown character ") << (uint32_t) report[2];
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSingleHidKeyboard
// @method:   getIdlePeriod
// @return:   The usb standard recommends a 500 ms periodic sampling for keyboards.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t
UsbDeviceSingleHidKeyboard::getIdlePeriod () const
{
  return 500;
}



