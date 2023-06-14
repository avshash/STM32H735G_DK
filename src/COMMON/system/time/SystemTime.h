#ifndef _SYSTEM_TIME_H_
#define _SYSTEM_TIME_H_

extern "C" void sysTickHandler ();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    SystemTime
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SystemTime
{
public:
  SystemTime ();

  static uint32_t getSystemMilliTime ();

private:
  // Note we need the '::' qualifier for an extern "C" friend.
  friend void ::sysTickHandler ();

  static uint32_t g_ticks_count;
};

#endif  // _SYSTEM_TIME_H_
