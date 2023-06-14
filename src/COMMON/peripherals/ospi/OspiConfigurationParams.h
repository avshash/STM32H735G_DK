#ifndef _OSPI_CONFIGURATION_PARAMS_H_
#define _OSPI_CONFIGURATION_PARAMS_H_

#include "TypeOspiMemory.h"
#include "TypeOspiConfigurationParam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    OspiConfigurationParams
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OspiConfigurationParams
{
public:
  OspiConfigurationParams (TypeOspiMemory);

  TypeOspiMemory getType () const;
  void setParam (TypeOspiConfigurationParam, uint32_t value);
  uint32_t getParam (TypeOspiConfigurationParam) const;

private:
  TypeOspiMemory m_memory_type;
  uint32_t m_params[OSPI_CONFIGURATION_PARAM_COUNT];
};

#endif  // _OSPI_CONFIGURATION_PARAMS_H_
