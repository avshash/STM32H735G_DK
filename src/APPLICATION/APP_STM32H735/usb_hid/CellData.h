#ifndef _CELL_DATA_H_
#define _CELL_DATA_H_

#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @struct:   CellData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CellData
{
  Timer m_timeout;
  uint16_t m_radius;
  uint16_t m_param_x0;
  uint16_t m_param_y0;
};

#endif  // _CELL_DATA_H_