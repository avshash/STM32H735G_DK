#include "Common.h"
#include "H735MpuManager.h"

#include "PhysicalRccRegisters.h"
#include "PhysicalSystemControlRegisters.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735MpuManager
// @method:   constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735MpuManager::H735MpuManager (uint32_t memory_base, uint32_t active_size, uint8_t mpu_region)
{
  RCC_REGISTERS.AHB3ENR = RCC_REGISTERS.AHB3ENR | 0x00000001UL;
  while ((RCC_REGISTERS.AHB3ENR & 0x00000001UL) == 0);

  uint32_t active_size_log = Common::log2int (active_size);
  ASSERT_CRITICAL ((4 < active_size_log) && (active_size_log <= 0x20));

  ASSERT_CRITICAL (MPU_REGISTERS.CTRL == 0);

  MPU_REGISTERS.RNR = mpu_region;
  MPU_REGISTERS.RBAR = memory_base;
  // Enable RW full access to non shareable normal memory.
  MPU_REGISTERS.RASR = 0x03020001UL | ((active_size_log - 1) << 1);
  // Enable MPU.
  MPU_REGISTERS.CTRL = 0x00000005UL;
}

