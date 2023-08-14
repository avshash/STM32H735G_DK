#include "Common.h"
#include "H735UsbPortManager.h"

#include "PhysicalOtgFsRegisters.h"
#include "ModuleManagerClockControl.h"
#include "PhysicalRccRegisters.h"
#include "PhysicalPwrRegisters.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
H735UsbPortManager::H735UsbPortManager (UsbChannelsManager & channels) :
  UsbPortManager (),
  m_interrupt_handler (channels)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   setupRcc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::setupRcc ()
{
  ModuleManagerClockControl & clock_control = ModuleManager::getClockControl ();
  clock_control.setClockFrequency (CLOCK_BUS_PLL3_Q, 48000000);

  ASSERT_TEST (clock_control.getClockFrequency (CLOCK_BUS_PLL3_Q) == 48000000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   configure
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::configure ()
{
  enableOtgClock ();
  enablePower ();

  m_interrupt_handler.configure ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   activatePort
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::activatePort ()
{
  setupFifo ();
  setupOtgFsLs ();

  // Clear active host-port interrupts. Clear the 'reset' bit if active.
  OTG_FS_REGISTERS.HPRT = OTG_FS_REGISTERS.HPRT & 0xFFFFFEFFUL;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   deactivatePort
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::deactivatePort ()
{
  // Do a complete core soft reset, just to be on the safe side.
  while ((OTG_FS_REGISTERS.GRSTCTL & 0x80000000UL) == 0);
  OTG_FS_REGISTERS.GRSTCTL = OTG_FS_REGISTERS.GRSTCTL | 0x01;
  while ((OTG_FS_REGISTERS.GRSTCTL & 0x01) == 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   enablePort
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::enablePort ()
{
  // Verify that interrupts are disabled.
  ASSERT_TEST ((OTG_FS_REGISTERS.GAHBCFG & 0x00000001UL) == 0);

  // Reset all active interrupts before unmasking the otg interrupts.
  OTG_FS_REGISTERS.GINTSTS = OTG_FS_REGISTERS.GINTSTS;

  // Clear active host-port interrupts (exclude the enabled bit).
  OTG_FS_REGISTERS.HPRT = OTG_FS_REGISTERS.HPRT & 0xFFFFFFFBUL;

  // Enable the start of frame interrupt, as well as the rxflvlm interrupt (incoming packet).
  OTG_FS_REGISTERS.GINTMSK = 0x00000018UL;

  // Unmask the otg interrupts.
  OTG_FS_REGISTERS.GAHBCFG = OTG_FS_REGISTERS.GAHBCFG | 0x00000001UL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   disablePort
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::disablePort ()
{
  // Mask the global interrupts bits.
  OTG_FS_REGISTERS.GAHBCFG = OTG_FS_REGISTERS.GAHBCFG & 0xFFFFFFFEUL;

  // Disable port (handles the case where we have a self powered enabled port).
  // Clear the 'reset' bit if active.
  OTG_FS_REGISTERS.HPRT = (OTG_FS_REGISTERS.HPRT & 0xFFFFFE51UL) | 0x00000004UL;

  while ((OTG_FS_REGISTERS.HPRT & 0x00000004UL) != 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   getPhysicalState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeUsbPortState
H735UsbPortManager::getPhysicalState ()
{
  uint32_t hprt_register = OTG_FS_REGISTERS.HPRT;

  // Clear active host-port interrupts (exclude the enabled bit).
  OTG_FS_REGISTERS.HPRT = hprt_register & 0xFFFFFFFBUL;

  if ((hprt_register & 0x00000004UL) != 0)
  {
    return USB_PORT_ENABLED;
  }

  if ((hprt_register & 0x00000001UL) == 0)
  {
    return USB_PORT_DISCONNECTED;
  }

  if ((hprt_register & 0x00000002UL) != 0)
  {
    // We do not care if device was just connected, or disconnected and immediately reconnected.
    // We mark it as disconnected. Next iteration would mark device as connected.
    return USB_PORT_DISCONNECTED;
  }

  return USB_PORT_CONNECTED;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   setReset
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::setReset (bool b_active)
{
  if (b_active)
  {
    // Enable 'reset' bit. Do not clear any flag.
    OTG_FS_REGISTERS.HPRT = (OTG_FS_REGISTERS.HPRT & 0xFFFFFED1UL) | 0x00000100UL;
  }
  else
  {
    if ((OTG_FS_REGISTERS.HPRT & 0x01UL) != 0)
    {
      // When a device is connected, set frame interval.
      bool b_port_full_speed = ((OTG_FS_REGISTERS.HPRT & 0x00060000UL) == 0x00020000UL);
      ASSERT_TEST (b_port_full_speed || ((OTG_FS_REGISTERS.HPRT & 0x00060000UL) == 0x00040000UL));

      OTG_FS_REGISTERS.HCFG = (OTG_FS_REGISTERS.HCFG & 0xFFFFFFFCUL) | (b_port_full_speed ? 0x01UL : 0x02UL);
      OTG_FS_REGISTERS.HFIR = b_port_full_speed ? 48000 : 6000;
    }

    // Disable 'reset' bit. Do not clear any flag.
    OTG_FS_REGISTERS.HPRT = OTG_FS_REGISTERS.HPRT & 0xFFFFFED1UL;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   testLowSpeed
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool
H735UsbPortManager::testLowSpeed () const
{
  bool b_port_low_speed = ((OTG_FS_REGISTERS.HPRT & 0x00060000UL) == 0x00040000UL);

  return b_port_low_speed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   enableOtgClock
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::enableOtgClock ()
{
  // Set clock source to pll3_q_ck.
  uint32_t d2ccip2r_base = RCC_REGISTERS.D2CCIP2R & 0xFFCFFFFFUL;;
  RCC_REGISTERS.D2CCIP2R = d2ccip2r_base | 0x00200000UL;

  // Enable clock for the usb peripheral.
  RCC_REGISTERS.AHB1ENR = RCC_REGISTERS.AHB1ENR | 0x02000000UL;
  while ((RCC_REGISTERS.AHB1ENR & 0x02000000UL) != 0x02000000UL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   setupOtgFsLs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::setupOtgFsLs ()
{
  // Configure the otg module to be in forced host mode, USB 1.1 full speed.
  OTG_FS_REGISTERS.GUSBCFG |= 0x20000040UL;
  // Must wait 25 ms for forcing host mode.
  Common::waitMicro (25000);

  // Enable full speed transceiver.
  OTG_FS_REGISTERS.GCCFG |= 0x00010000UL;

  // Support only full / low speed.
  OTG_FS_REGISTERS.HCFG |= 0x00000004UL;

  // Set port power on.
  OTG_FS_REGISTERS.HPRT |= 0x00001000UL;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   setupFifo
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::setupFifo ()
{
  // Set the fifo alert level to half empty.
  OTG_FS_REGISTERS.GAHBCFG = OTG_FS_REGISTERS.GAHBCFG & 0xFFFFFE7FFUL;

  // The dedicated fifo ram is 4Kb.
  // We let the rx fifo hold 2048 bytes, and both periodic and non-periodic fifo hold 1024 bytes.
  OTG_FS_REGISTERS.GRXFSIZ  = 0x200U;
  OTG_FS_REGISTERS.HNPTXFSIZ = ((0x100UL << 16) ) | 0x200UL;
  OTG_FS_REGISTERS.HPTXFSIZ = (0x100UL << 16) | 0x300U;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @method:   enablePower
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
H735UsbPortManager::enablePower ()
{
  // Enable the voltage level detector, and disable the internal usb regulator.
  PWR_REGISTERS.CR3 = (PWR_REGISTERS.CR3 & 0xFDFFFFFFUL) | 0x01000000UL;
}


