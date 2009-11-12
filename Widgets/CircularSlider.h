// On screen display - circular slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_WIDGETS_CIRCULAR_SLIDER_
#define _OE_UTILS_WIDGETS_CIRCULAR_SLIDER_

#include <Widgets/IWidget.h>
#include <Resources/IFontTextureResource.h>

#include <Math/Vector.h>
#include <Core/Event.h>

#include <string>

namespace OpenEngine {
namespace Widgets {

using Core::Event;

using std::string;
class WidgetRenderer;

class ValueChangedEvent{
public:
    float value;
    ValueChangedEvent(float value): value(value) {}
};

/**
 * On Screen Display Circular Slider Class.  
 *
 * A circular slider changes value in a predefined interval when
 * manipulated by the input device.
 *
 * @class CircularSlider CircularSlider.h OpenGLSelection/Utils/CircularSlider.h
 */
class CircularSlider: public IWidget {
private:
    Event<ValueChangedEvent> e;
    Resources::IFontTextureResourcePtr val_texr;
    Resources::IFontTextureResourcePtr text_texr;
    int x, y, width, height;
    bool active, focus;
    float value, startAngle, sweep, step;
    Math::Vector<2,float> center, focusVec;
    string text;
    // int AngleToQuadrant(float angle);
public:
    Event<ValueChangedEvent>& ChangedEvent() {return e;}

    void SetupFonts(WidgetRenderer& r);

    explicit CircularSlider();
    CircularSlider(WidgetRenderer& r);
    virtual ~CircularSlider();

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
    float GetValue();
    void SetValue(float value);
    string GetText();
    void SetText(string text);
    float GetStartAngle();
    float GetSweep();
    Resources::ITextureResourcePtr GetValueTexture();
    Resources::ITextureResourcePtr GetTextTexture();
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS__CIRCULAR_SLIDER_
