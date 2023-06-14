#ifndef _FLASH_H_
#define _FLASH_H_

#include "TypeFlashParam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Flash
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Flash
{
public:
  virtual uint32_t getFlashParam (TypeFlashParam) const = 0;
  virtual void readData (uint32_t address, uint8_t * buffer, uint32_t bytes_count) = 0;
  virtual void writeData (uint32_t address, const uint8_t * buffer, uint32_t bytes_count) = 0;

// 'resetMemoryRange' resets (erases) memory in range ['range_start', range_limit' - 1].
// Both 'range_start', and 'range_limit' must be a multiple of 'FLASH_PARAM_SECTOR_SIZE'.
  virtual void resetMemoryRange (uint32_t range_start, uint32_t range_limit) = 0;
};

#endif  // _FLASH_H_
