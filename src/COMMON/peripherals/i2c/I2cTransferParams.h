#ifndef _I2C_TRANSFER_PARAMS_H_
#define _I2C_TRANSFER_PARAMS_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   I2cTransferParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct I2cTransferParams
{

  uint8_t m_device_address;
  uint8_t m_bytes_to_send;
  uint8_t m_bytes_to_receive;
  uint8_t m_buffer_offset;
  uint8_t m_errors_count;
  uint8_t m_buffer[15];
};

#endif  // _I2C_TRANSFER_PARAMS_H_
