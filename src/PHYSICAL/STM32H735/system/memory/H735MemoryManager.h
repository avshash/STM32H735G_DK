#ifndef _H735_MEMORY_MANAGER_H_
#define _H735_MEMORY_MANAGER_H_

#include "MemoryManager.h"
#include "PhysicalMdmaRegisters.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735MemoryManager
// @dscr:     Documented by PM0468 Pragramming manual, section 14.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735MemoryManager : public MemoryManager
{
public:
  H735MemoryManager (int channel);

private:
  virtual void enable () final;

  PhysicalMdmaRegisters & m_mdma_register;
};

typedef H735MemoryManager PhysicalMemoryManager;

#endif  // _H735_MEMORY_MANAGER_H_
