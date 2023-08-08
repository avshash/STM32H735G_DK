#ifndef _USB_PORT_MANAGER_H_
#define _USB_PORT_MANAGER_H_

#include "UsbPort.h"
#include "TypeUsbPortState.h"
#include "Timer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    UsbPortManager
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UsbPortManager : protected UsbPort
{
public:
  void tick ();
  virtual TypeUsbPortState getPortState () const final;

  virtual void configure () = 0;
  virtual void activateGpio () = 0;
  virtual void setupRcc () = 0;

protected:
  UsbPortManager ();

private:
  void setPortState (TypeUsbPortState, uint32_t lease_time = 0);
  void powerDownTest ();
  void tickPoweredDown ();
  void tickDisconnected ();
  void tickEnumerationPending ();
  void tickConnected ();
  void tickEnabled ();
  void tickPowerDownPending ();
  virtual void portResetAction (bool b_reset_active) final;

  virtual void enablePort () = 0;
  virtual void setReset (bool b_active) = 0;
  virtual TypeUsbPortState getPhysicalState () = 0;
  virtual void setupPower (bool b_active) = 0;
  virtual bool testOverCurrentActive () = 0;

  volatile TypeUsbPortState m_port_state;
  Timer m_timeout;
  uint32_t m_enumerating_session;
};

#endif  // _USB_PORT_MANAGER_H_
