#ifndef _DISC735_MODULE_MANAGER_USB_H_
#define _DISC735_MODULE_MANAGER_USB_H_

#include "ModuleManagerUsb.h"
#include "Disc735UsbPortManager.h"
#include "PhysicalUsbChannelsManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerUsb
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735ModuleManagerUsb : public ModuleManagerUsb
{
public:
  Disc735ModuleManagerUsb ();

private:
  PhysicalUsbChannelsManager m_channels;
  Disc735UsbPortManager m_port_manager;
};

typedef Disc735ModuleManagerUsb PlatformModuleManagerUsb;

#endif  // _DISC735_MODULE_MANAGER_USB_H_

