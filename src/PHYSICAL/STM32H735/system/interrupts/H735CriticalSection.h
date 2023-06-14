#ifndef _H735_CRITICAL_SECTION_H_
#define _H735_CRITICAL_SECTION_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @dscr:     This class disables interrupts on constructor, and resumes interrupts on destructor.
// @note:     Do not disable fundemental sysTick (quick) interrupt.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735CriticalSection
{
public:
  H735CriticalSection ();
  ~H735CriticalSection ();

private:
};

typedef H735CriticalSection PhysicalCriticalSection;

#endif  // _H735_CRITICAL_SECTION_H_
