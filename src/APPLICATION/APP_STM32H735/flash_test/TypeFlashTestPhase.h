#ifndef _TYPE_FLASH_TEST_PHASE_H_
#define _TYPE_FLASH_TEST_PHASE_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeFlashTestPhase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeFlashTestPhase
{
  FLASH_TEST_PHASE_INIT,
  FLASH_TEST_PHASE_TEST_ERASE_CHIP_0,
  FLASH_TEST_PHASE_TEST_WRITE_PAGE_1,
  FLASH_TEST_PHASE_TEST_VERIFY_PAGE_1,
  FLASH_TEST_PHASE_TEST_ERASE_CHIP_1,
  FLASH_TEST_PHASE_TEST_WRITE_PAGE_2,
  FLASH_TEST_PHASE_TEST_VERIFY_PAGE_2,
  FLASH_TEST_PHASE_TEST_ERASE_CHIP_2,
  FLASH_TEST_PHASE_TEST_GOOD
};

#endif  // _TYPE_FLASH_TEST_PHASE_H_

