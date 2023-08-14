#include "Common.h"
#include "UsbDeviceSinglePrinterEsc.h"

#include "ModuleManagerLcd.h"
#include "ModuleManagerFlash.h"
#include "Flash.h"
#include "TextSingleLine.h"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbDeviceSinglePrinterEsc::UsbDeviceSinglePrinterEsc  () :
  UsbDeviceSinglePrinter ()
{
  memset (&m_flash_data, 0, sizeof (m_flash_data));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   clearPrinterBuffer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::clearPrinterBuffer ()
{
  m_flash_data.m_blocks_count = 2000 / sizeof (m_buffer);

  parseNextPacket (USB_PRINTER_CLEARING_BUFFER);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   printSingleLine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::printSingleLine (const char * text)
{
  if (getPrinterState () != USB_PRINTER_IDLE)
  {
    ASSERT_TEST (false);
    return;
  }

  uint8_t * buffer = (uint8_t *) m_buffer;
  int text_len = strlen (text);
  text_len = MIN (text_len, (int) (sizeof (m_buffer) - 1));

  memcpy (buffer, text, text_len);
  buffer[text_len] = '\n';

  sendPrinterPacket (m_buffer, text_len + 1, USB_PRINTER_LAST_PACKET);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   parseNextPacket
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::parseNextPacket (TypeUsbPrinterState printer_state)
{
  switch (printer_state)
  {
    case USB_PRINTER_CLEARING_BUFFER:
      return printClearingBuffer ();

    case USB_PRINTER_LAST_PACKET:
      return;

    case USB_PRINTER_IMAGE:
      return printImage ();

    case USB_PRINTER_IDLE:
    case USB_PRINTER_ERROR:
    case USB_PRINTER_CLOSED:
      break;
  }

  ASSERT_TEST (false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   printClearingBuffer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::printClearingBuffer ()
{
  if (0 < m_flash_data.m_blocks_count)
  {
    memset (m_buffer, 0, sizeof (m_buffer));
    sendPrinterPacket (m_buffer, sizeof (m_buffer), USB_PRINTER_CLEARING_BUFFER);
    m_flash_data.m_blocks_count--;
  }
  else
  {
    printCutPaper ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   printCutPaper
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::printCutPaper ()
{
  const uint8_t command_cut_output[] =
  {
    0x1B, 0x4A, 0x18,                                               // Feeds 24 points vertical.
    0x1D, 0x56, 0x01,                                               // Cut paper.
    0x1B, 0x4A, 0x18,                                               // Feeds 24 points vertical.
    0x1B, 0x40                                                      // Reset printer state.
  };
  uint8_t * buffer = (uint8_t *) m_buffer;

  memcpy (buffer, command_cut_output, sizeof (command_cut_output));
  sendPrinterPacket (m_buffer, sizeof (command_cut_output), USB_PRINTER_LAST_PACKET);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   printQrCode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::printQrCode (const char * qr_code, uint16_t qr_code_length)
{
  const uint8_t command_qr_code_prefix[] =
  {
    0x0A,                                                           // Line feed.
    0x1D, 0x28, 0x6B, 0x04, 0x00, 0x31, 0x41, 0x32, 0x00,           // Select the QR model 2 mode.
    0x1D, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x43, 0x08,                 // Sets the QR code size to 8 dots.
    0x1D, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x45, 0x32,                 // Sets the QR correction level to 'Q'.
    0x1D, 0x28, 0x6B, 0x00, 0x00, 0x31, 0x50, 0x30                  // Store QR data in printer.
  };

  const uint8_t command_qr_code_postfix[] =
  {
    0x1D, 0x28, 0x6B, 0x03, 0x00, 0x31, 0x51, 0x30,                 // Flush buffer.
    0x0A                                                            // Line feed.
  };

  uint16_t reminder = sizeof (m_buffer) - (sizeof (command_qr_code_prefix) + sizeof (command_qr_code_postfix));
  qr_code_length = MIN (qr_code_length, reminder);

  uint8_t * buffer = (uint8_t *) m_buffer;
  memcpy (buffer, command_qr_code_prefix, sizeof (command_qr_code_prefix));

  buffer += sizeof (command_qr_code_prefix);
  buffer[-5] = 3 + (qr_code_length & 0x00FF);
  buffer[-4] = qr_code_length / 0x0100;
  memcpy (buffer, qr_code, qr_code_length);

  buffer += qr_code_length;
  memcpy (buffer, command_qr_code_postfix, sizeof (command_qr_code_postfix));

  buffer += sizeof (command_qr_code_postfix);
  uint16_t print_size = buffer - (const uint8_t *) m_buffer;

  sendPrinterPacket (m_buffer, print_size, USB_PRINTER_LAST_PACKET);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   printRawData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::printRawData (uint8_t x_size, uint16_t y_size, uint32_t flash_address_base)
{
  uint16_t blocks_count = (y_size + 0x17) / 0x18;
  if ((sizeof (m_buffer) < x_size) || (0x100 <= blocks_count))
  {
    ASSERT_TEST (false);
    return;
  }

  memset (&m_flash_data, 0, sizeof (m_flash_data));
  m_flash_data.m_flash_address = flash_address_base;
  m_flash_data.m_blocks_count = blocks_count & 0x00FFU;
  m_flash_data.m_x_size = x_size;

  const uint8_t command_cut_output[] =
  {
    0x1B, 0x40,                                                     // Reset printer state.
    0x0A                                                            // Line feed.
  };
  uint8_t * buffer = (uint8_t *) m_buffer;

  memcpy (buffer, command_cut_output, sizeof (command_cut_output));
  sendPrinterPacket (m_buffer, sizeof (command_cut_output), USB_PRINTER_IMAGE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   printImage
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::printImage ()
{
  if (m_flash_data.m_blocks_count == 0)
  {
    return printCutPaper ();
  }

  if (0x18 < m_flash_data.m_y_offset)
  {
    m_flash_data.m_blocks_count--;
    m_flash_data.m_y_offset = 0;
    return printImageTail ();
  }

  m_flash_data.m_y_offset++;

  if (m_flash_data.m_y_offset == 1)
  {
    return printImageHeader ();
  }

  printImageBody ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   printImageBody
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::printImageBody ()
{
  ModuleManagerFlash & flash_manager = ModuleManager::getFlash ();
  Flash & flash_interface = flash_manager.getFlashInterface ();

  uint32_t address = m_flash_data.m_flash_address;
  uint32_t bytes_count = m_flash_data.m_x_size;

  if (flash_interface.getFlashParam (FLASH_PARAM_MEMORY_SIZE) < (address + bytes_count))
  {
    ASSERT_TEST (false);
    memset (m_buffer, 0, bytes_count);
  }
  else
  {
    flash_interface.readData (address, (uint8_t *) m_buffer, bytes_count);
    m_flash_data.m_flash_address = address + bytes_count;
  }

  sendPrinterPacket (m_buffer, bytes_count, USB_PRINTER_IMAGE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   printImageTail
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::printImageTail ()
{
  const uint8_t command_image_tail[] =
  {
    0x1B, 0x4A, 0x18                                                // Print image block (and advance feed paper).
  };

  memcpy (m_buffer, command_image_tail, sizeof (command_image_tail));
  sendPrinterPacket (m_buffer, sizeof (command_image_tail), USB_PRINTER_IMAGE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbDeviceSinglePrinterEsc
// @method:   printImageHeader
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbDeviceSinglePrinterEsc::printImageHeader ()
{
  const uint8_t command_image_header[] =
  {
    // Set image header with 180x180 dpi. Block vertical size is 24 bits.
    0x1D, 0x76, 0x30, 0x00, 0x00, 0x00, 0x18, 0x00
  };
  uint8_t * buffer = (uint8_t *) m_buffer;

  memcpy (buffer, command_image_header, sizeof (command_image_header));
  buffer[4] = m_flash_data.m_x_size;
  sendPrinterPacket (m_buffer, sizeof (command_image_header), USB_PRINTER_IMAGE);
}

