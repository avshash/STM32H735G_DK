#ifndef _MODULE_MANAGER_USB_H_
#define _MODULE_MANAGER_USB_H_

#include "ModuleManager.h"
#include "UsbDevicesManager.h"

class UsbPortManager;
class UsbChannelsManager;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ModuleManagerUsb
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ModuleManagerUsb : public ModuleManager
{
public:
  UsbPortManager & getPortManager ();
  UsbChannelsManager & getChannelsManager ();
  UsbDevicesManager & getDevicesManager ();

protected:
  ModuleManagerUsb (UsbPortManager &, UsbChannelsManager &);

private:
  virtual void doAction (TypeModuleAction) final;
  void tick ();

  UsbPortManager & m_port_manager;
  UsbChannelsManager & m_channels_manager;
  UsbDevicesManager m_devices_manager;
};

#endif  // _MODULE_MANAGER_USB_H_
