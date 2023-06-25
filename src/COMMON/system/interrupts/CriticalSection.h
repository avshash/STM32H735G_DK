#ifndef _CRITICAL_SECTION_H_
#define _CRITICAL_SECTION_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    CriticalSection
// @dscr:     This class disables interrupts on constructor, and resumes interrupts on destructor.
// @note:     Do not disable fundemental sysTick (quick) interrupt.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CriticalSection
{
public:
  CriticalSection ();

  void handleInterrupt ();
  void setPrevActive (CriticalSection *);
  CriticalSection * getPrevActive ();

protected:
  void setEnabling ();

private:
  virtual void enableInterrupts () = 0;

  bool m_enabling;
  CriticalSection * m_prev_active;
};

#endif  // _CRITICAL_SECTION_H_
