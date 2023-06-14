#ifndef _MACRONIX_PARAMS_H_
#define _MACRONIX_PARAMS_H_

#include "TypeFlashParam.h"
#include "TypeMacronixParam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    MacronixParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MacronixParams
{
public:
  MacronixParams ();

  void setMacronixParam (TypeMacronixParam, uint32_t value);
  uint32_t getMacronixParam (TypeMacronixParam) const;
  void setFlashParam (TypeFlashParam, uint32_t value);
  uint32_t getFlashParam (TypeFlashParam) const;
  bool verifyInitiated () const;

private:
  uint32_t m_macronix_params[MACRONIX_PARAM_COUNT];
  uint32_t m_flash_params[FLASH_PARAM_COUNT];
};

#endif  // _MACRONIX_PARAMS_H_
