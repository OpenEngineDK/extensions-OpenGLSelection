// On screen display - slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_WIDGETS_SLIDER_
#define _OE_UTILS_WIDGETS_SLIDER_

#include <Widgets/IWidget.h>

#include <Math/Vector.h>

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
// template <class ValueType>
typedef float ValueType;
class Slider: public IWidget {
private:
    int x, y, width, height;
    bool active, focus;
    ValueType value;
public:
    Slider();
    virtual ~Slider();

    Math::Vector<2,int> GetPosition();
    Math::Vector<2,int> GetDimensions();
    void SetPosition(Math::Vector<2,int> pos);
    void SetDimensions(Math::Vector<2,int> dim);
    void Accept(IWidgetRenderer& r);
    bool GetActive();
    void SetActive(bool active);
    bool GetFocus();
    void SetFocus(bool focus);
    IWidget* WidgetAt(int x, int y);
    IWidget* FocusAt(int x, int y);
    IWidget* ActivateAt(int x, int y);
    IWidget* ActivateFocus();
    void Reset();
    ValueType GetValue();
    void SetValue(ValueType value);
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_WIDGETS_SLIDER_
