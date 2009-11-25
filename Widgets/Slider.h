// On screen display - slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WIDGETS_SLIDER_
#define _OE_WIDGETS_SLIDER_

#include <Widgets/IWidget.h>
#include <Widgets/ValueWidget.h>
#include <Math/Vector.h>
#include <Core/Event.h>

namespace OpenEngine {
namespace Widgets {
/**
 * On Screen Display Slider Class.  
 *
 * A slider changes value in the interval [0.0,1.0] when manipulated
 * by the input device.
 *
 * @class Slider Slider.h OpenGLSelection/Utils/Slider.h
 */
class Slider: public ValueWidget<float> {
private:
public:
    Slider();
    virtual ~Slider();

    void Accept(IWidgetRenderer& r);
    IWidget* WidgetAt(int x, int y);
    IWidget* FocusAt(int x, int y);
    IWidget* ActivateAt(int x, int y);
    IWidget* ActivateFocus();
    void Reset();
    float GetValue();
    void SetValue(float value);
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_WIDGETS_SLIDER_
