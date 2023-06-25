#include "Common.h"
#include "H735CriticalSection.h"

#include "ModuleManagerInterrupts.h"
#include "PhysicalSystemControlRegisters.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   constructor
// @note:     We have only 8 words, so it's best not to use a loop.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735CriticalSection::H735CriticalSection ()
{
  ModuleManagerInterrupts & interrupts_manager = ModuleManager::getInterrupts ();
  interrupts_manager.criticalSection (this);

  registerState ();
  disableInterrupts ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735CriticalSection::~H735CriticalSection ()
{
  setEnabling ();

  enableInterrupts ();

  ModuleManagerInterrupts & interrupts_manager = ModuleManager::getInterrupts ();
  interrupts_manager.criticalSection (NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   enableInterrupts
// @note:     We have only 8 words, so it's best not to use a loop.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735CriticalSection::enableInterrupts ()
{
  volatile uint32_t * target = NVIC_REGISTERS.NVIC_ISER;
  uint32_t * iser_vector = m_iser_vector;

  target[0] = iser_vector[0];
  target[1] = iser_vector[1];
  target[2] = iser_vector[2];
  target[3] = iser_vector[3];
  target[4] = iser_vector[4];
  target[5] = iser_vector[5];
  target[6] = iser_vector[6];
  target[7] = iser_vector[7];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   registerState
// @note:     We have only 8 words, so it's best not to use a loop.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735CriticalSection::registerState ()
{
  volatile uint32_t * state = NVIC_REGISTERS.NVIC_ISER;
  uint32_t * iser_vector = m_iser_vector;

  iser_vector[0] = state[0];
  iser_vector[1] = state[1];
  iser_vector[2] = state[2];
  iser_vector[3] = state[3];
  iser_vector[4] = state[4];
  iser_vector[5] = state[5];
  iser_vector[6] = state[6];
  iser_vector[7] = state[7];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735CriticalSection
// @method:   disableInterrupts
// @note:     We have only 8 words, so it's best not to use a loop.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735CriticalSection::disableInterrupts ()
{
  volatile uint32_t * disable = NVIC_REGISTERS.NVIC_ICER;

  disable[0] = 0xFFFFFFFFUL;
  disable[1] = 0xFFFFFFFFUL;
  disable[2] = 0xFFFFFFFFUL;
  disable[3] = 0xFFFFFFFFUL;
  disable[4] = 0xFFFFFFFFUL;
  disable[5] = 0xFFFFFFFFUL;
  disable[6] = 0xFFFFFFFFUL;
  disable[7] = 0xFFFFFFFFUL;
}


