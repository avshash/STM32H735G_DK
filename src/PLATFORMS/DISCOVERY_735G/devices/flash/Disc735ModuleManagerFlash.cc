#include "Common.h"
#include "Disc735ModuleManagerFlash.h"

#include "PhysicalOctoSpi.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerFlash
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Disc735ModuleManagerFlash::Disc735ModuleManagerFlash () :
  ModuleManagerFlash (m_flash_manager),
  m_physical (),
  m_macronix_params (),
  m_flash_manager (m_physical, m_macronix_params)
{
  // MX25LM51245G
  m_macronix_params.setMacronixParam (MACRONIX_PARAM_MANUFACTORER_ID, 0xC2);
  m_macronix_params.setMacronixParam (MACRONIX_PARAM_MEMORY_TYPE, 0x85);
  m_macronix_params.setMacronixParam (MACRONIX_PARAM_MEMORY_DENSITY, 0x3A);
  m_macronix_params.setMacronixParam (MACRONIX_PARAM_CLOCK_MAX_FREQUENCY, 84000000);
  m_macronix_params.setMacronixParam (MACRONIX_PARAM_DUMMY_CYCLES, 8);

  m_macronix_params.setFlashParam (FLASH_PARAM_MEMORY_SIZE, 0x04000000UL);
  m_macronix_params.setFlashParam (FLASH_PARAM_BLOCK_SIZE, 0x00010000UL);
  m_macronix_params.setFlashParam (FLASH_PARAM_SECTOR_SIZE, 0x00001000UL);
  m_macronix_params.setFlashParam (FLASH_PARAM_PAGE_SIZE, 0x00000100UL);
}


