#include "Common.h"
#include "H735MemoryManager.h"

#include "PhysicalSystemControlRegisters.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735MemoryManager
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735MemoryManager::H735MemoryManager (int channel) :
  m_mdma_register (MDMA_REGISTER (channel))
{
  ASSERT_CRITICAL ((0 <= channel) && (channel < 0x10));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735MemoryManager
// @method:   enable
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735MemoryManager::enable ()
{
  ASSERT_CRITICAL ((m_mdma_register.CxCR & 0x01) == 0);

  // Set priority level to high.
  m_mdma_register.CxCR = 0x00000080UL;
  // Mdma is setup to bufferable, software mode, block transfer, right aligned, packed, 128 bytes buffer transfer,
  // No burst transfer, all transfers are word sized (4 bytes).
  m_mdma_register.CxTCR = 0xD3FC0AAAUL;
  m_mdma_register.CxBNDTR = 0;
  m_mdma_register.CxBRUR = 0;
  m_mdma_register.CxTBR = 0;
  m_mdma_register.CxLAR = 0;
}


