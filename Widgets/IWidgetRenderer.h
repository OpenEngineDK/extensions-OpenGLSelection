// On screen display - interface for an OSD renderer.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WIDGETS_INTERFACE_WIDGET_RENDERER_
#define _OE_WIDGETS_INTERFACE_WIDGET_RENDERER_

namespace OpenEngine {
namespace Widgets {

class Button;
class Slider;
template <class T>
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
    virtual void Visit(Button* w) = 0;
    virtual void Visit(Slider* w) = 0;
    virtual void Visit(CircularSlider<float>* w) = 0;
    virtual void Visit(CircularSlider<int>* w) = 0;
    virtual void Visit(Collection* w) = 0;
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_WIDGETS_RENDERER_
