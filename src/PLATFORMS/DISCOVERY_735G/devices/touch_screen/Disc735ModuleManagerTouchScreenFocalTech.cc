#include "Common.h"
#include "Disc735ModuleManagerTouchScreenFocalTech.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735ModuleManagerTouchScreenFocalTech
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Disc735ModuleManagerTouchScreenFocalTech::Disc735ModuleManagerTouchScreenFocalTech (ListenerTouchScreen & listener) :
  ModuleManagerTouchScreenFocalTech (listener, m_interface, true),
  m_interface ()
{
}


