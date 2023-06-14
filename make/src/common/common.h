#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include "OutOfMemory.h"
#include "InputError.h"
#include "InternalError.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @enum:     TypeErrorColor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum TypeErrorColor
{
  ERROR_COLOR_WHITE,
  ERROR_COLOR_RED,
  ERROR_COLOR_BLUE,
  ERROR_COLOR_GREEN,
  ERROR_COLOR_PURPLE,
  ERROR_COLOR_YELLOW
};

void printError (const char *, TypeErrorColor color = ERROR_COLOR_RED) noexcept;
void printError (const char *, const char *, TypeErrorColor color = ERROR_COLOR_RED) noexcept;
void printError (const char *, const char *, const char *, TypeErrorColor color = ERROR_COLOR_RED) noexcept;
void printError (const char *, const char *, int, TypeErrorColor color = ERROR_COLOR_RED) noexcept;
void printError (const char *, int, TypeErrorColor color = ERROR_COLOR_RED) noexcept;
void printError (const char *, int, int, TypeErrorColor color = ERROR_COLOR_RED) noexcept;

#endif // _COMMON_H_
