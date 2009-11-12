// On screen display - interface for an OSD renderer.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_WIDGETS_INTERFACE_WIDGET_RENDERER_
#define _OE_UTILS_WIDGETS_INTERFACE_WIDGET_RENDERER_

namespace OpenEngine {
namespace Widgets {

class Button;
class Slider;
class CircularSlider;
class Collection;

/**
 * On Screen Display Renderer Class.
 *
 * @class IWidgetRenderer IWidgetRenderer.h OpenGLSelection/Utils/IWidgetRenderer.h
 */
class IWidgetRenderer {
private:
public:
    virtual ~IWidgetRenderer() {};
    virtual void Render(Button& w) = 0;
    virtual void Render(Slider& w) = 0;
    virtual void Render(CircularSlider& w) = 0;
    virtual void Render(Collection& w) = 0;
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_WIDGETS_RENDERER_
