#ifndef _H735_OCTO_SPI_H_
#define _H735_OCTO_SPI_H_

#include "OctoSpi.h"
#include "H735OctoSpiRegisters.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735OctoSpi
// @dscr:     Behavior is documented by PM0468 Pragramming manual, section 25.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735OctoSpi : public OctoSpi
{
protected:
  H735OctoSpi (H735OctoSpiRegisters &);

  virtual void configure (const OspiConfigurationParams & params) final;

private:
  virtual unsigned int commitWriteCommand (const OspiCommandParams &) final;
  virtual unsigned int commitReadCommand (OspiCommandParams &) final;
  virtual uint32_t getDirectMemoryBase () const final;
  virtual uint32_t readHyperbusRegister (uint32_t address) final;
  virtual void writeHyperbusRegister (uint32_t address, uint32_t register_value) final;
  virtual void verifyMemoryMappedGood () final;

  void configureDcr1 (TypeOspiMemory, bool b_memory_mapped, uint32_t device_log_size, uint32_t chip_select_high_time);
  void configureDcr2 (uint32_t max_clock_frequency);
  void configureDcr3 (uint32_t cs_bound_log_size);
  void configureDcr4 (uint32_t refresh_rate);
  void configureTcr (uint32_t b_dhqc_active);
  void configureHlcr (uint32_t rw_recovery_time, uint32_t access_time, bool b_fixed_latency, bool b_write_zero_latency);
  void configureCr (bool b_memory_mapped, uint32_t fifo_thresthod_level);
  void configureHyperbusCcr ();
  void enableSpiClock ();
  void criticalWrite (uint32_t const * buffer);
  void readReply (uint32_t * buffer);
  void setCommandParams (const OspiCommandParams &);
  void cleanupTransfer ();
  void setCcrRegister (const OspiCommandParams &);
  unsigned int dataSize () const;

  H735OctoSpiRegisters & m_octo_spi_registers;
};

typedef H735OctoSpi PhysicalOctoSpi;

#endif  // _H735_OCTO_SPI_H_

