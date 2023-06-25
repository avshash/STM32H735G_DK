#ifndef _H735_CRITICAL_SECTION_H_
#define _H735_CRITICAL_SECTION_H_

#include "CriticalSection.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735CriticalSection : public CriticalSection
{
public:
  H735CriticalSection ();
  ~H735CriticalSection ();

private:
  virtual void enableInterrupts () final;
  void registerState ();
  void disableInterrupts ();

  uint32_t m_iser_vector[8];
};

typedef H735CriticalSection PhysicalCriticalSection;

#endif  // _H735_CRITICAL_SECTION_H_
