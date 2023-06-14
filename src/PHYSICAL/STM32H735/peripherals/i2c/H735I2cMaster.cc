#include "Common.h"
#include "H735I2cMaster.h"

#include "ModuleManagerClockControl.h"
#include "PhysicalRccRegisters.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMaster
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735I2cMaster::H735I2cMaster (H735I2cRegisters & i2c_registers) :
  m_i2c_registers (i2c_registers)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMaster
// @method:   configure
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMaster::configure (const uint32_t params[I2C_CONFIGURATION_PARAMS_COUNT])
{

  clockEnable ();

  // Support only 100 Khz.
  ASSERT_CRITICAL (params[I2C_CONFIGURATION_PARAM_FREQUENCY] == 100000);
  configureTimingR100K ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMaster
// @method:   clockEnable
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMaster::clockEnable ()
{
  // Enable clock.
  if (&m_i2c_registers == &I2C_4_REGISTERS)
  {
    RCC_REGISTERS.APB4ENR = RCC_REGISTERS.APB4ENR | 0x00000080UL;
    while ((RCC_REGISTERS.APB4ENR & 0x00000080UL) == 0);
    return;
  }

  if (&m_i2c_registers == &I2C_1_REGISTERS)
  {
    RCC_REGISTERS.APB1LENR = RCC_REGISTERS.APB1LENR | 0x00200000UL;
    while ((RCC_REGISTERS.APB1LENR & 0x00200000UL) == 0);
    return;
  }

  if (&m_i2c_registers == &I2C_2_REGISTERS)
  {
    RCC_REGISTERS.APB1LENR = RCC_REGISTERS.APB1LENR | 0x00400000UL;
    while ((RCC_REGISTERS.APB1LENR & 0x00400000UL) == 0);
    return;
  }

  if (&m_i2c_registers == &I2C_3_REGISTERS)
  {
    RCC_REGISTERS.APB1LENR = RCC_REGISTERS.APB1LENR | 0x00800000UL;
    while ((RCC_REGISTERS.APB1LENR & 0x00800000UL) == 0);
    return;
  }

  if (&m_i2c_registers == &I2C_5_REGISTERS)
  {
    RCC_REGISTERS.APB1LENR = RCC_REGISTERS.APB1LENR | 0x02000000UL;
    while ((RCC_REGISTERS.APB1LENR & 0x02000000UL) == 0);
    return;
  }

  ASSERT_CRITICAL (false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMaster
// @method:   read
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
H735I2cMaster::read (I2cMessage & message_params)
{
  ASSERT_CRITICAL (0 < message_params.m_data_size);
  ASSERT_TEST ((m_i2c_registers.CR1 & 0x01) == 0);
  m_i2c_registers.CR1 = 0x00000001UL;

  // As data is limited to 5 bytes, 2 millisecond (160 bits) is a reasonable limit at 100Khz.
  m_action_timeout.leaseFromNow (10);

  int ret_val = setupDataAddress (message_params);

  if (0 <= ret_val)
  {
    ret_val = readData (message_params);
  }

  if (0 <= ret_val)
  {
    if (!waitIsrBit (0x20))
    {
      ret_val = -1;
    }
  }

  m_i2c_registers.CR2 = 0;
  // Disabling the peripheral resets the internal state machine, as well as status bits.
  m_i2c_registers.CR1 = 0;

  ASSERT_TEST (0 < ret_val);
  return ret_val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMaster
// @method:   setupDataAddress
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
H735I2cMaster::setupDataAddress (I2cMessage & message_params)
{
  // Support only 7 bits device address.
  ASSERT_CRITICAL ((m_i2c_registers.CR2 & 0x00000800UL) == 0);

  // Support only a single byte data address.
  uint32_t data_address_size_mask = 0x00010000UL;
  uint32_t device_address_mask = 2 * message_params.m_device_address;
  m_i2c_registers.CR2 = 0x00002000UL | data_address_size_mask | device_address_mask;

  // Wait for device ack.
  if (!waitIsrBit (0x02))
  {
    return -101;
  }

  m_i2c_registers.TXDR = message_params.m_data_address;
  // Wait for transfer complete.
  if (!waitIsrBit (0x40UL))
  {
    return -102;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMaster
// @method:   readData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int
H735I2cMaster::readData (I2cMessage & message_params)
{
  uint32_t bytes_count = message_params.m_data_size;

  // We have a 255 bytes limit for each transaction.
  ASSERT_TEST (sizeof (message_params.m_message_data) < 256);
  ASSERT_TEST (message_params.m_data_size <= sizeof (message_params.m_message_data));
  ASSERT_CRITICAL ((0 < bytes_count) && (bytes_count <= sizeof (message_params.m_message_data)));

  m_i2c_registers.CR2 = 0x02012470UL | (bytes_count << 16);

  for (uint32_t offset = 0; offset < bytes_count; offset++)
  {
    if (!waitIsrBit (0x04))
    {
      return -201;
    }

    message_params.m_message_data[offset] = m_i2c_registers.RXDR;
  }

  return message_params.m_data_size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMaster
// @method:   waitIsrBit
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
H735I2cMaster::waitIsrBit (uint32_t bit_mask)
{
  while ((m_i2c_registers.ISR & bit_mask) != bit_mask)
  {
    if ((m_i2c_registers.ISR & 0x00000210U) != 0)
    {
      return false;
    };

    if (m_action_timeout.testExpired ())
    {
      return false;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735I2cMaster
// @method:   configureTimingR100K
// @note:     All time measurements are in pico-seconds.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735I2cMaster::configureTimingR100K ()
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
// @class:    H735I2cMaster
// @method:   getPclkPeriod
// @dscr:     Calculates the kernel clocks' period in pico seconds.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t
H735I2cMaster::getPclkPicoPeriod () const
{
  ModuleManagerClockControl & clock_control = ModuleManager::getClockControl ();

  // I2C number 4 is running on D3 domain. All the others run on D2 domain.
  uint32_t pclk_frequency = (&m_i2c_registers == &I2C_4_REGISTERS) ? clock_control.getClockFrequency (CLOCK_BUS_PCLK4)
                                                                   : clock_control.getClockFrequency (CLOCK_BUS_PCLK1);

  // Kernel clock must be faster than the I2C clock.
  ASSERT_CRITICAL (1000000 <= pclk_frequency);

  return (1000000000UL / (pclk_frequency / 1000));
}

