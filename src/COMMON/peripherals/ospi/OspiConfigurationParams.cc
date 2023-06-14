#include "Common.h"
#include "OspiConfigurationParams.h"

#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    OspiConfigurationParams
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OspiConfigurationParams::OspiConfigurationParams (TypeOspiMemory memory_type) :
  m_memory_type (memory_type)
{
  memset (m_params, 0, sizeof (m_params));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    OspiConfigurationParams
// @method:   getType
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeOspiMemory
OspiConfigurationParams::getType () const
{
  return m_memory_type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    OspiConfigurationParams
// @method:   setParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
OspiConfigurationParams::setParam (TypeOspiConfigurationParam param_name, uint32_t value)
{
  ASSERT_TEST (m_params[param_name] == 0);

  m_params[param_name] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    OspiConfigurationParams
// @method:   getParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
OspiConfigurationParams::getParam (TypeOspiConfigurationParam param_name) const
{
  return m_params[param_name];
}
