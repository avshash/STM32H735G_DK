#ifndef _DISC735_USB_PORT_MANAGER_H_
#define _DISC735_USB_PORT_MANAGER_H_

#include "PhysicalUsbPortManager.h"
#include "PhysicalGpioAf.h"
#include "PhysicalGpioOut.h"
#include "PhysicalGpioIn.h"

class UsbChannelsManager;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735UsbPortManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Disc735UsbPortManager : public PhysicalUsbPortManager
{
public:
  Disc735UsbPortManager (UsbChannelsManager &);

private:
  virtual void activateGpio () final;
  virtual void setupPower (bool b_active) final;
  virtual bool testOverCurrentActive () final;

  PhysicalGpioAf m_fs_dm;
  PhysicalGpioAf m_fs_dp;
  PhysicalGpioOut m_fs_pwr_en;
  PhysicalGpioIn m_fs_ovcr;
};

#endif  // _DISC735_USB_PORT_MANAGER_H_

