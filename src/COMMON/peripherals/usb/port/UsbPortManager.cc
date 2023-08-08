#include "Common.h"
#include "UsbPortManager.h"

#include "ModuleManagerUsb.h"
#include "UsbDevicesManager.h"
#include "UsbChannelsManager.h"
#include "TextSingleLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbPortManager::UsbPortManager () :
  m_port_state (USB_PORT_POWERED_DOWN),
  m_timeout ()
{
  // Enabled usb port 1 second after power on.
  m_timeout.leaseFromNow (1000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   powerDownTest
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::powerDownTest ()
{
  if (testOverCurrentActive ())
  {
    setupPower (false);

    switch (m_port_state)
    {
      case USB_PORT_POWERED_DOWN:
        m_timeout.extendLease (1000);
        break;

      case USB_PORT_POWER_DOWN_PENDING:
        break;

      default:
        setPortState (USB_PORT_POWER_DOWN_PENDING);
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   setPortState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::setPortState (TypeUsbPortState new_port_state, uint32_t lease_time)
{
  const char * verbose[] = 
  {
    "'powered down'",
    "'disconnected'",
    "'enumeration pending'",
    "'reset active'",
    "'connected'",
    "'enabled'",
    "'powered down'"
  };

  if (m_port_state == USB_PORT_ENABLED)
  {
    disablePort ();
    ASSERT_TEST (new_port_state == USB_PORT_POWER_DOWN_PENDING);
  }

  TextSingleLine ().print ("Change port state from ") << verbose[m_port_state] <<
                           " to " << verbose[new_port_state] << ".";

  m_port_state = new_port_state;
  m_timeout.leaseFromNow (lease_time);

  if (new_port_state == USB_PORT_ENABLED)
  {
    enablePort ();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   getPortState
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TypeUsbPortState
UsbPortManager::getPortState () const
{
  return m_port_state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   portResetAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::portResetAction (bool b_reset_active)
{
  setReset (b_reset_active);

  if (b_reset_active)
  {
    ASSERT_TEST (m_port_state == USB_PORT_ENUMERATION_PENDING);

    setPortState (USB_PORT_RESET_ACTIVE);
  }
  else
  {
    ASSERT_TEST (m_port_state == USB_PORT_RESET_ACTIVE);

    // Wait for the device no more than 100 ms.
    setPortState (USB_PORT_CONNECTED, 100);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   tick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::tick ()
{
  powerDownTest ();

  TypeUsbPortState port_state = m_port_state;
  switch (port_state)
  {
    case USB_PORT_POWERED_DOWN:
      tickPoweredDown ();
      break;

    case USB_PORT_DISCONNECTED:
      tickDisconnected ();
      break;

    case USB_PORT_ENUMERATION_PENDING:
      tickEnumerationPending ();
      break;

    case USB_PORT_RESET_ACTIVE:
      break;

    case USB_PORT_CONNECTED:
      tickConnected ();
      break;

    case USB_PORT_ENABLED:
      tickEnabled ();
      break;

    case USB_PORT_POWER_DOWN_PENDING:
      tickPowerDownPending ();
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   tickPoweredDown
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::tickPoweredDown ()
{
  setupPower (false);

  if (m_timeout.testExpired ())
  {
    setPortState (USB_PORT_DISCONNECTED);
    setupPower (true);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   tickDisconnected
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::tickDisconnected ()
{
  // Enable power, just in case.
  setupPower (true);

  TypeUsbPortState physical_state = getPhysicalState ();

  if (physical_state == USB_PORT_CONNECTED)
  {
    // Wait 100 ms between physical connection and the enumeration process.
    setPortState (USB_PORT_ENUMERATION_PENDING, 100);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   tickConnected
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::tickConnected ()
{
  TypeUsbPortState physical_state = getPhysicalState ();

  switch (physical_state)
  {
    case USB_PORT_DISCONNECTED:
      setPortState (USB_PORT_POWER_DOWN_PENDING);
      break;

    case USB_PORT_ENABLED:
      setPortState (USB_PORT_ENABLED);
      break;

    case USB_PORT_CONNECTED:
      if (m_timeout.testExpired ())
      {
        setPortState (USB_PORT_POWER_DOWN_PENDING);
      }
      break;

    default:
      ASSERT_CRITICAL (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   tickEnumerationPending
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::tickEnumerationPending ()
{
  TypeUsbPortState physical_state = getPhysicalState ();

  switch (physical_state)
  {
    case USB_PORT_DISCONNECTED:
      setPortState (USB_PORT_POWER_DOWN_PENDING);
      break;

    case USB_PORT_CONNECTED:
      {
        // Enumerator would initiate a 'reset'.
        ModuleManagerUsb & module_usb = ModuleManager::getUsb ();
        UsbDevicesManager & devices_manager = module_usb.getDevicesManager ();
        devices_manager.portConnected (this);
      }
      break;

    default:
      ASSERT_CRITICAL (false);
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   tickEnabled
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::tickEnabled ()
{
  TypeUsbPortState physical_state = getPhysicalState ();

  if (physical_state != USB_PORT_ENABLED)
  {
    setPortState (USB_PORT_POWER_DOWN_PENDING);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
// @method:   tickPowerDownPending
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UsbPortManager::tickPowerDownPending ()
{
  setupPower (false);

  // Keep port powered down for 1 second before setting power back on.
  setPortState (USB_PORT_POWERED_DOWN, 1000);
}

