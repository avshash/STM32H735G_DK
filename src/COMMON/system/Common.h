#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stddef.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common functions.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Common
{
  void assertPrint (const char *, uint32_t, uint32_t);
  void waitMicro (uint32_t micro_seconds);
  unsigned int log2int (uint32_t variable);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COMMON MACROS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FILE_UNIQUE_ID is defined by the compiler command line.

#if (ASSERT_LEVEL_ACTIVE <= 0x30U)

#define ASSERT_INTERNAL(assert_level, assert_statement)                 \
  if ((assert_level) <= ASSERT_LEVEL_ACTIVE)                            \
  {                                                                     \
    bool assert_val = (assert_statement);                               \
    while (!assert_val)                                                 \
    {                                                                   \
      const uint32_t id = _FILE_UNIQUE_ID + __LINE__;                   \
      Common::assertPrint (__FILE__, __LINE__, id);                     \
      assert_val = (assert_statement);                                  \
    }                                                                   \
  }

#else     // 0x30 < ASSERT_LEVEL_ACTIVE

#define ASSERT_INTERNAL(assert_level, assert_statement)                 \
  while (!(assert_statement))                                           \
  {                                                                     \
    while (1);                                                          \
  }

#endif    // ASSERT_LEVEL_ACTIVE

#define ASSERT_TEMP(assert_statement)                 ASSERT_INTERNAL (0x30U, (assert_statement))
#define ASSERT_DEV(assert_statement)                  ASSERT_INTERNAL (0x20U, (assert_statement))
#define ASSERT_TEST(assert_statement)                 ASSERT_INTERNAL (0x10U, (assert_statement))
#define ASSERT_CRITICAL(assert_statement)             ASSERT_INTERNAL (0x00U, (assert_statement))

#define MIN(a,b)                                      ((a) < (b) ? (a) : (b))
#define MAX(a,b)                                      ((a) < (b) ? (b) : (a))
#define U8VAL16(ptr, offset)                          ((uint16_t)(((const uint8_t *)(ptr))[offset]))
#define U8LE2U16(ptr)                                 (U8VAL16((ptr), 0) + (U8VAL16((ptr), 1) << 8))

#endif // _COMMON_H_

