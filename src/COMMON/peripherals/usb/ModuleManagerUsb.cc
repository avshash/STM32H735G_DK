#include "Common.h"
#include "ModuleManagerUsb.h"

#include "UsbPortManager.h"
#include "UsbChannelsManager.h"
#include "UsbDevicesManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerUsb
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModuleManagerUsb::ModuleManagerUsb (UsbPortManager & port_manager, UsbChannelsManager & channels_manager) :
  ModuleManager (MODULE_MANAGER_USB),
  m_port_manager (port_manager),
  m_channels_manager (channels_manager),
  m_devices_manager ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerUsb
// @method:   doAction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerUsb::doAction (TypeModuleAction action)
{
  switch (action)
  {
    case MODULE_ACTION_TICK:
      tick ();
      break;

    case MODULE_ACTION_CONFIGURE_PLL_CLOCKS:
      m_port_manager.setupRcc ();
      break;

    case MODULE_ACTION_ACTIVATE_GPIO:
      m_port_manager.activateGpio ();
      break;

    case MODULE_ACTION_START:
      m_port_manager.configure ();
      break;

    case MODULE_ACTION_OPEN:
    case MODULE_ACTION_START_PLL_CLOCKS:
    case MODULE_ACTION_START_MDMA:
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerUsb
// @method:   getPortManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbPortManager &
ModuleManagerUsb::getPortManager ()
{
  return m_port_manager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerUsb
// @method:   getChannelsManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbChannelsManager &
ModuleManagerUsb::getChannelsManager ()
{
  return m_channels_manager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerUsb
// @method:   getDevicesManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UsbDevicesManager &
ModuleManagerUsb::getDevicesManager ()
{
  return m_devices_manager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerUsb
// @method:   tick
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
ModuleManagerUsb::tick ()
{
  m_port_manager.tick ();
  m_channels_manager.tick ();
  m_devices_manager.tick ();
}

