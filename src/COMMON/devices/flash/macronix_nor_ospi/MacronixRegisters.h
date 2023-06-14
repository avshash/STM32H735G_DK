#ifndef _MACRONIX_REGISTERS_H_
#define _MACRONIX_REGISTERS_H_

class MacronixInterface;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixRegisters
// @dscr:     Configure the flash chip to OPI mode.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MacronixRegisters
{
public:
  MacronixRegisters (MacronixInterface &);

  void setupStrMode (uint32_t chip_id_target);
  void strSetMemoryProtection (bool b_enable);

private:
  bool testMacronixModeSpi (uint32_t chip_id_target);
  bool testMacronixModeStr (uint32_t chip_id_target);
  void setupRegisters ();
  uint8_t spiReadConfigurationRegister2 (uint32_t address);
  uint8_t strReadConfigurationRegister2 (uint32_t address);
  void spiWriteConfigurationRegister2 (uint32_t address, uint8_t value);
  void strWriteConfigurationRegister2 (uint32_t address, uint8_t value);
  void setupDummyCycles ();
  void verifySecurityRegister ();
  void setupStatusRegister ();
  void spiSetMemoryProtection (bool b_enable);

  MacronixInterface & m_interface;
};

#endif  // _MACRONIX_REGISTERS_H_
