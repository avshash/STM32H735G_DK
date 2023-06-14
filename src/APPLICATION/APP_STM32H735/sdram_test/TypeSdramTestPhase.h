#ifndef _TYPE_SDRAM_TEST_PHASE_H_
#define _TYPE_SDRAM_TEST_PHASE_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeSdramTestPhase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeSdramTestPhase
{
  SDRAM_TEST_PHASE_INIT,
  SDRAM_TEST_PHASE_TEST_SET_00,
  SDRAM_TEST_PHASE_VERIFY_SET_00,
  SDRAM_TEST_PHASE_TEST_SET_FF,
  SDRAM_TEST_PHASE_VERIFY_SET_FF,
  SDRAM_TEST_PHASE_TEST_SEQUENCE,
  SDRAM_TEST_PHASE_VERIFY_SEQUENCE,
  SDRAM_TEST_PHASE_TEST_RANDOM,
  SDRAM_TEST_PHASE_VERIFY_RANDOM,
  SDRAM_TEST_PHASE_TEST_GOOD
};

#endif  // _TYPE_SDRAM_TEST_PHASE_H_

