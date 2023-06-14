#ifndef _TYPE_MACRONIX_COMMAND_H_
#define _TYPE_MACRONIX_COMMAND_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeMacronixCommand
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeMacronixCommand
{
  // SPI mode.
  MACRONIX_COMMAND_SPI_READ4B,
  MACRONIX_COMMAND_SPI_PP4B,
  MACRONIX_COMMAND_SPI_SE4B,
  MACRONIX_COMMAND_SPI_WREN,
  MACRONIX_COMMAND_SPI_RDID,
  MACRONIX_COMMAND_SPI_RDSR,
  MACRONIX_COMMAND_SPI_RDCR,
  MACRONIX_COMMAND_SPI_WRSR,
  MACRONIX_COMMAND_SPI_RDCR2,
  MACRONIX_COMMAND_SPI_WRCR2,
  MACRONIX_COMMAND_SPI_RDSCUR,
  MACRONIX_COMMAND_SPI_NOP,
  // OPI mode.
  MACRONIX_COMMAND_OPI_8READ,
  MACRONIX_COMMAND_OPI_RDID,
  MACRONIX_COMMAND_OPI_PP,
  MACRONIX_COMMAND_OPI_SE,
  MACRONIX_COMMAND_OPI_WREN,
  MACRONIX_COMMAND_OPI_RDSR,
  MACRONIX_COMMAND_OPI_RDCR,
  MACRONIX_COMMAND_OPI_WRSR_WRCR,
  MACRONIX_COMMAND_OPI_RDCR2,
  MACRONIX_COMMAND_OPI_WRCR2,
  MACRONIX_COMMAND_OPI_RDSCUR,
  MACRONIX_COMMAND_OPI_NOP
};

#endif  // _TYPE_MACRONIX_COMMAND_H_