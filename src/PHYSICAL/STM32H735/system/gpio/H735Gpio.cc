#include "Common.h"
#include "H735Gpio.h"

#include "PhysicalGpioRegisters.h"
#include "ModuleManagerPhysical.h"
#include "ModuleManagerClockControl.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735Gpio::H735Gpio (TypeGpioBank bank, int pin_index) :
  m_bank (bank),
  m_pin_index (pin_index)
{
  ASSERT_DEV ((0 <= pin_index) && (pin_index < 16));

  setupClock ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
// @method:   destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735Gpio::~H735Gpio ()
{
  setPortMode (GPIO_MODE_ANALOG);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
// @method:   setPortMode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Gpio::setPortMode (TypeGpioMode mode_type)
{
  PhysicalGpioRegisters & gpio_registers = GPIO_REGISTERS_VECTOR[m_bank];
  int shift_bits = 2 * m_pin_index;

  uint32_t moder_base = gpio_registers.MODER & (~(0x03UL << shift_bits));
  uint32_t moder_value = 0;

  switch (mode_type)
  {
    case GPIO_MODE_INPUT:
      break;

    case GPIO_MODE_OUTPUT:
      moder_value = 0x01UL << shift_bits;
      break;

    case GPIO_MODE_AF:
      moder_value = 0x02UL << shift_bits;
      break;

    case GPIO_MODE_ANALOG:
      moder_value = 0x03UL << shift_bits;
      break;
  }

  gpio_registers.MODER = moder_base | moder_value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
// @method:   setOutputType
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Gpio::setOutputType (TypeGpioOutput output_type)
{
  PhysicalGpioRegisters & gpio_registers = GPIO_REGISTERS_VECTOR[m_bank];

  uint32_t otyper_base = gpio_registers.OTYPER & (~(0x01UL << m_pin_index));
  uint32_t otyper_value = 0;

  switch (output_type)
  {
    case GPIO_OUTPUT_PUSH_PULL:
      break;

    case GPIO_OUTPUT_OPEN_DRAIN:
      otyper_value = 0x01UL << m_pin_index;
      break;
  }

  gpio_registers.OTYPER = otyper_base | otyper_value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
// @method:   setOutputSpeed
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Gpio::setOutputSpeed (TypeGpioSpeed speed_type)
{
  PhysicalGpioRegisters & gpio_registers = GPIO_REGISTERS_VECTOR[m_bank];
  int shift_bits = 2 * m_pin_index;

  uint32_t ospeedr_base = gpio_registers.OSPEEDR & (~(0x03UL << shift_bits));
  uint32_t ospeedr_value = 0;

  switch (speed_type)
  {
    case GPIO_SPEED_LOW:
      break;

    case GPIO_SPEED_MEDIUM:
      ospeedr_value = 0x01UL << shift_bits;
      break;

    case GPIO_SPEED_HIGH:
      ospeedr_value = 0x02UL << shift_bits;
      break;

    case GPIO_SPEED_VERY_HIGH:
      ospeedr_value = 0x03UL << shift_bits;
      break;
  }

  gpio_registers.OSPEEDR = ospeedr_base | ospeedr_value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
// @method:   setLoadResistor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Gpio::setLoadResistor (TypeGpioLoad load_type)
{
  PhysicalGpioRegisters & gpio_registers = GPIO_REGISTERS_VECTOR[m_bank];
  int shift_bits = 2 * m_pin_index;

  uint32_t load_base = gpio_registers.PUPDR & (~(0x03UL << shift_bits));
  uint32_t load_value = 0;

  switch (load_type)
  {
    case GPIO_LOAD_DISCONNECTED:
      break;

    case GPIO_LOAD_PULL_UP:
      load_value = 0x01UL << shift_bits;
      break;

    case GPIO_LOAD_PULL_DOWN:
      load_value = 0x02UL << shift_bits;
      break;
  }

  gpio_registers.PUPDR = load_base | load_value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
// @method:   setOutput
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Gpio::setOutput (bool b_value)
{
  if (b_value)
  {
    GPIO_REGISTERS_VECTOR[m_bank].BSRR = 0x01UL << m_pin_index;
  }
  else
  {
    GPIO_REGISTERS_VECTOR[m_bank].BSRR = 0x01UL << (m_pin_index + 16);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
// @method:   setupClock
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Gpio::setupClock ()
{
  ASSERT_DEV ((GPIO_BANK_A <= m_bank) && (m_bank <= GPIO_BANK_K));

  ModuleManagerClockControl & clock_control = ModuleManager::getClockControl ();
  clock_control.activateGpioClock (m_bank);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735Gpio
// @method:   setAfMode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735Gpio::setAfMode (int af_index)
{
  ASSERT_TEST (af_index < 16);

  H735GpioRegisters & gpio_registers = GPIO_REGISTERS_VECTOR[m_bank];

  uint32_t register_index = m_pin_index / 8;
  uint32_t shift_bits = ((m_pin_index % 8) * 4);

  uint32_t af_register_base = gpio_registers.AFR[register_index] & (~(0x0FUL << shift_bits));
  uint32_t af_value = af_index << shift_bits;

  gpio_registers.AFR[register_index] = af_register_base | af_value;
}


