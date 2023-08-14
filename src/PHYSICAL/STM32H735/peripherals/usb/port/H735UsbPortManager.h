#ifndef _H735_USB_PORT_MANAGER_H_
#define _H735_USB_PORT_MANAGER_H_

#include "UsbPortManager.h"
#include "PhysicalUsbChannelsManager.h"
#include "PhysicalUsbInterruptsHandler.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    H735UsbPortManager
// @dscr:     Behavior is documented by PM0468 Pragramming manual, section 62.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class H735UsbPortManager : public UsbPortManager
{
public:
  H735UsbPortManager (UsbChannelsManager &);

private:
  // 'UsbPortManager' implementation.
  virtual void setupRcc () final;
  virtual void configure () final;
  virtual bool testLowSpeed () const final;
  virtual void activatePort () final;
  virtual void deactivatePort () final;
  virtual void disablePort () final;
  virtual void enablePort () final;
  virtual void setReset (bool b_active) final;
  virtual TypeUsbPortState getPhysicalState () final;

  void enableOtgClock ();
  void setupOtgFsLs ();
  void enablePower ();
  void setupFifo ();

  PhysicalUsbInterruptHandler m_interrupt_handler;
};

typedef H735UsbPortManager PhysicalUsbPortManager;

#endif  // _H735_USB_PORT_MANAGER_H_

