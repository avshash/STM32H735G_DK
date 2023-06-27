#ifndef _H735_CRITICAL_SECTION_H_
#define _H735_CRITICAL_SECTION_H_

#include "TypeCriticalSection.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735CriticalSection
{
public:
  H735CriticalSection ();
  ~H735CriticalSection ();

private:
  void registerActiveInterrupts ();
  void resumeInterrupts ();
  void disableInterrupts ();

  TypeCriticalSection m_state;
  H735CriticalSection * m_prev_active;
  volatile uint32_t m_iser_vector[6];

  volatile static H735CriticalSection * m_active_critical_section;
};

typedef H735CriticalSection PhysicalCriticalSection;

#endif  // _H735_CRITICAL_SECTION_H_
