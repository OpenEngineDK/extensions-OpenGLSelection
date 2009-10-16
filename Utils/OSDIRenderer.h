// On screen display - interface for an OSD renderer.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_OSD_INTERFACE_RENDERER_
#define _OE_UTILS_OSD_INTERFACE_RENDERER_

namespace OpenEngine {
namespace Utils {

class OSDButton;
class OSDSlider;
class OSDCollection;

/**
 * On Screen Display Renderer Class.
 *
 * @class OSDRenderer OSDRenderer.h OpenGLSelection/Utils/OSDRenderer.h
 */
class OSDIRenderer {
private:
public:
    virtual ~OSDIRenderer() {};
    virtual void Render(OSDButton& w) = 0;
    virtual void Render(OSDSlider& w) = 0;
    virtual void Render(OSDCollection& w) = 0;
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_OSD_RENDERER_
