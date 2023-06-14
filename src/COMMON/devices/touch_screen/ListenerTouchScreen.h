#ifndef _LISTENER_TOUCH_SCREEN_H_
#define _LISTENER_TOUCH_SCREEN_H_

#include "TouchData.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    ListenerTouchScreen
// @dscr:     Notifies listener about touch change.
// @dscr:     Invoked during a module tick (not by an interrupt).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ListenerTouchScreen
{
public:
  virtual void registerAction (const TouchData & last_data, const TouchData & next_data) = 0;
};

#endif  // _LISTENER_TOUCH_SCREEN_H_
