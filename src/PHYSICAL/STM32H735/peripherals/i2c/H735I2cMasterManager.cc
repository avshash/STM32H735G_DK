#include "Common.h"
#include "H735I2cMasterManager.h"

#include "ModuleManagerClockControl.h"
#include "ModuleManagerInterrupts.h"
#include "PhysicalRccRegisters.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735I2cMasterManager::H735I2cMasterManager (H735I2cRegisters & i2c_registers) :
  m_i2c_registers (i2c_registers)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   configure
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMasterManager::configure (uint32_t frequency)
{
  clockEnable ();

  // Support only 100 Khz.
  ASSERT_CRITICAL (frequency == 100000);
  configureTimingR100K ();
  registerInterruptHandler ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   setupTransfer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMasterManager::setupTransfer (uint8_t device_address, uint8_t data_size, bool b_last_action, bool b_action_send)
{
  m_i2c_registers.CR1 = 0x000000B1UL |
                        (b_last_action ? 0 : 0x00000040UL) |
                        (b_action_send ? 0x00000002UL : 0x00000004UL);
  m_i2c_registers.CR2 = (b_last_action ? 0x02000000UL : 0) |
                        (((uint32_t) data_size) << 16) |
                        0x00002000UL |
                        (b_action_send ? 0x00000000UL : 0x00000400UL) |
                        (((uint32_t) device_address) << 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   disablePeripheral
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMasterManager::disablePeripheral ()
{
  m_i2c_registers.CR1 = 0x00000000UL;

  // Wait until the start bit is disabled.
  while ((m_i2c_registers.CR2 & 0x00002000UL) != 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   getActiveInterrupt
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeI2cInterrupt
H735I2cMasterManager::getActiveInterrupt ()
{
  uint32_t isr_register = m_i2c_registers.ISR;

  if ((isr_register & 0x00001F00UL) != 0)
  {
    m_i2c_registers.ICR = 0x00001F00UL;
    return I2C_INTERRUPT_ERROR;
  }

  if ((isr_register & 0x00000040UL) != 0)
  {
    return I2C_INTERRUPT_TRANSFER_COMPLETE;
  }

  if ((isr_register & 0x00000004UL) != 0)
  {
    m_i2c_registers.ICR = 0x00000004UL;
    return I2C_INTERRUPT_RX_ACTIVE;
  }

  if ((isr_register & 0x00000002UL) != 0)
  {
    m_i2c_registers.ICR = 0x00000002UL;
    return I2C_INTERRUPT_TX_ACTIVE;
  }

  if ((isr_register & 0x00000020UL) != 0)
  {
    m_i2c_registers.ICR = 0x00000020UL;
    return I2C_INTERRUPT_STOP_DETECTION;
  }

  if ((isr_register & 0x00000010UL) != 0)
  {
    m_i2c_registers.ICR = 0x00000010UL;
    return I2C_INTERRUPT_NACK;
  }

  return I2C_INTERRUPT_NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   setNextTxValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMasterManager::setNextTxValue (uint8_t to_send)
{
  m_i2c_registers.TXDR = to_send;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   getNextRxValue
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t
H735I2cMasterManager::getNextRxValue ()
{
  return m_i2c_registers.RXDR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   clockEnable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMasterManager::clockEnable ()
{
  int index = getI2cIndex ();

  if (index == 4)
  {
    RCC_REGISTERS.APB4ENR = RCC_REGISTERS.APB4ENR | 0x00000080UL;
    while ((RCC_REGISTERS.APB4ENR & 0x00000080UL) == 0);
  }
  else
  {
    uint32_t mask = 0x00100000UL << index;

    RCC_REGISTERS.APB1LENR = RCC_REGISTERS.APB1LENR | mask;
    while ((RCC_REGISTERS.APB1LENR & mask) == 0);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   configureTimingR100K
// @note:     All time measurements are in pico-seconds.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMasterManager::configureTimingR100K ()
{
  uint32_t pclk_pico_period = getPclkPicoPeriod ();

  // Set t_presc to be about 100 nsec.
  uint32_t presc_ticks = (100000 + (pclk_pico_period / 2)) / pclk_pico_period;
  presc_ticks = MIN (16, presc_ticks);
  presc_ticks = MAX (1, presc_ticks);
  uint32_t t_presc = presc_ticks * pclk_pico_period;
  ASSERT_CRITICAL ((80000 <= t_presc) && (t_presc <= 120000));

  // Consult RM0468 section 52.4.5 for complete constraints list.
  // Most are trivial for relevant setups, when t_presc is about 100 nsec.
  // Set t_sdadel to be at least 500 nsec, and t_scldel to be at most 1250 nsec (as per table 405 example in RM0468).
  uint32_t sdadel_ticks = (500000 + t_presc - 1) / t_presc;
  uint32_t scldel_ticks = (1250000 / t_presc);

  // Calculate sclh, and scll according to RM0468, section 52.4.9 - I2C master mode.
  // Substract twice t_sync from the 10 usec period, and divide by t_presc to get the number of active ticks.
  uint32_t t_sync = 50000 + (3 * t_presc);
  uint32_t active_ticks = (10000000 - (2 * t_sync)) / t_presc;
  uint32_t scll_ticks = 5000000 / t_presc;
  uint32_t sclh_ticks = active_ticks - scll_ticks;

  ASSERT_CRITICAL ((4700000 <= (scll_ticks * t_presc)) && (4000000 <= (sclh_ticks * t_presc)));

  m_i2c_registers.TIMINGR = ((presc_ticks - 1) << 28) |
                            ((scldel_ticks - 1) << 20) |
                            (sdadel_ticks << 16) |
                            ((sclh_ticks - 1) << 8) |
                            (scll_ticks - 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   registerInterruptHandler
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMasterManager::registerInterruptHandler ()
{
  ModuleManagerInterrupts & interrupts_manager = ModuleManager::getInterrupts ();
  interrupts_manager.registerInterrupt (*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   getI2cIndex
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
H735I2cMasterManager::getI2cIndex () const
{
  if (&m_i2c_registers == &I2C_1_REGISTERS)
  {
    return 1;
  }

  if (&m_i2c_registers == &I2C_2_REGISTERS)
  {
    return 2;
  }

  if (&m_i2c_registers == &I2C_3_REGISTERS)
  {
    return 3;
  }

  if (&m_i2c_registers == &I2C_4_REGISTERS)
  {
    return 4;
  }

  if (&m_i2c_registers == &I2C_5_REGISTERS)
  {
    return 5;
  }

  ASSERT_CRITICAL (false);
  return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   getPclkPicoPeriod
// @dscr:     Calculates the kernel clocks' period in pico seconds.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735I2cMasterManager::getPclkPicoPeriod () const
{
  ModuleManagerClockControl & clock_control = ModuleManager::getClockControl ();

  // I2C number 4 is running on D3 domain. All the others run on D2 domain.
  uint32_t pclk_frequency = (&m_i2c_registers == &I2C_4_REGISTERS) ? clock_control.getClockFrequency (CLOCK_BUS_PCLK4)
                                                                   : clock_control.getClockFrequency (CLOCK_BUS_PCLK1);

  // Kernel clock must be faster than the I2C clock.
  ASSERT_CRITICAL (1000000 <= pclk_frequency);

  return (1000000000UL / (pclk_frequency / 1000));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   getInterruptParam
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
H735I2cMasterManager::getInterruptParam (TypeInterruptParam param)
{
  switch (param)
  {
    case INTERRUPT_PARAM_IRQ_ENTRIES_COUNT:
      return 2;

    case INTERRUPT_PARAM_IRQ_PRIORITY:
      return 7;

    case INTERRUPT_PARAM_HANDLER_GROUP:
      return INTERRUPT_HANDLER_I2C4;
  }

  ASSERT_CRITICAL (false);
  return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMasterManager
// @method:   getIrqNumber
// @dscr:     Follows table 140, in RM0468.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
H735I2cMasterManager::getIrqNumber (int irq_index)
{
  const int nvic_i2c[] = {31, 33, 72, 95, 157};
  ASSERT_TEST ((0 <= irq_index) && (irq_index <= 1));

  int i2c_index = getI2cIndex () - 1;
  ASSERT_TEST ((0 <= irq_index) && (irq_index < (int) (sizeof (nvic_i2c) / sizeof (int))));

  return (nvic_i2c[i2c_index] + irq_index);
}

