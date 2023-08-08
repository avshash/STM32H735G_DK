#include "Common.h"
#include "H735CriticalSection.h"

#include "PhysicalSystemControlRegisters.h"

volatile H735CriticalSection * H735CriticalSection::m_active_critical_section = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   constructor
// @note:     We have only 6 active words, so it's best not to use a loop.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735CriticalSection::H735CriticalSection () :
  m_state (CRITICAL_SECTION_REGISTER),
  m_prev_active ((H735CriticalSection *) m_active_critical_section)
{
  if ((m_prev_active != NULL) && (m_prev_active->m_state == CRITICAL_SECTION_RESUME))
  {
    // Handle the case where a low priority resumed interrupt preempts a high priority disabled interrupt.
    m_prev_active->resumeInterrupts ();
  }

  m_active_critical_section = this;

  registerActiveInterrupts ();
  disableInterrupts ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735CriticalSection::~H735CriticalSection ()
{
  m_state = CRITICAL_SECTION_RESUME;

  resumeInterrupts ();
  m_active_critical_section = m_prev_active;

  if ((m_prev_active != NULL) && (m_prev_active->m_state == CRITICAL_SECTION_REGISTER))
  {
    // Handle the case where a critical section activated or disabled some handler.
    // Note that the previous active object is still suspended, as we have higher priority.
    m_prev_active->registerActiveInterrupts ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   resumeInterrupts
// @note:     We have only 6 active words, so it's best not to use a loop.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735CriticalSection::resumeInterrupts ()
{
  volatile uint32_t * target = NVIC_REGISTERS.NVIC_ISER;
  volatile uint32_t * iser_vector = m_iser_vector;

  target[0] = iser_vector[0];
  target[1] = iser_vector[1];
  target[2] = iser_vector[2];
  target[3] = iser_vector[3];
  target[4] = iser_vector[4];
  target[5] = iser_vector[5];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   registerActiveInterrupts
// @note:     We have only 6 active words, so it's best not to use a loop.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735CriticalSection::registerActiveInterrupts ()
{
  volatile uint32_t * state = NVIC_REGISTERS.NVIC_ISER;
  volatile uint32_t * iser_vector = m_iser_vector;

  iser_vector[0] = state[0];
  iser_vector[1] = state[1];
  iser_vector[2] = state[2];
  iser_vector[3] = state[3];
  iser_vector[4] = state[4];
  iser_vector[5] = state[5];

  m_state = CRITICAL_SECTION_ACTIVE;

  ASSERT_TEST ((state[6] == 0) && (state[7] == 0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   disableInterrupts
// @note:     We have only 6 active words, so it's best not to use a loop.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735CriticalSection::disableInterrupts ()
{
  volatile uint32_t * disable = NVIC_REGISTERS.NVIC_ICER;

  disable[5] = 0xFFFFFFFFUL;
  disable[4] = 0xFFFFFFFFUL;
  disable[3] = 0xFFFFFFFFUL;
  disable[2] = 0xFFFFFFFFUL;
  disable[1] = 0xFFFFFFFFUL;
  disable[0] = 0xFFFFFFFFUL;
}


