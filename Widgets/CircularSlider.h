// On screen display - circular slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WIDGETS_CIRCULAR_SLIDER_
#define _OE_WIDGETS_CIRCULAR_SLIDER_

#include <Widgets/IWidget.h>
#include <Widgets/IWidgetRenderer.h>
#include <Resources/IFontTextureResource.h>
#include <Math/Math.h>
#include <Math/Vector.h>
#include <Core/Event.h>

#include <string>

namespace OpenEngine {
namespace Widgets {

using Core::Event;
using std::string;
using Math::Vector;

template <class T>
class CircularSlider;

template <class T> 
class ValueChangedEventArg {
public:
    T value;
    CircularSlider<T>* widget;
    ValueChangedEventArg(T value, CircularSlider<T>* widget): value(value), widget(widget) {}
};

/**
 * On Screen Display Circular Slider Class.  
 *
 * A circular slider changes value in a predefined interval when
 * manipulated by the input device.
 *
 * @class CircularSlider CircularSlider.h OpenGLSelection/Widgets/CircularSlider.h
 */
template <class T>
class CircularSlider: public IWidget {
private:
    Event<ValueChangedEventArg<T> > e;
    int x, y, width, height;
    bool active, focus;
    T value, step;
    float startAngle, sweep;
    Math::Vector<2,float> center, focusVec;
    // int AngleToQuadrant(float angle);
public:
    Event<ValueChangedEventArg<T> >& ValueChangedEvent() { return e; }

    CircularSlider(T value, T step)
        : x(0)
        , y(0)
        , width(0)
        , height(0)
        , active(false)
        , focus(false)
        , value(value)
        , step(step)
    {}
    virtual ~CircularSlider() {}
    
    Vector<2,int> GetPosition() {
        return Vector<2,int>(x, y);
    }

    Vector<2,int> GetDimensions() {
        return Vector<2,int>(width, height);
    }

    void SetPosition(Vector<2,int> pos) {
        x = pos[0];
        y = pos[1];
        center = Vector<2,float>(x + 0.5 * width, y + 0.5 * height);
    }

    void SetDimensions(Vector<2,int> dim) {
        width = dim[0];
        height = dim[1];
        center = Vector<2,float>(x + 0.5 * width, y + 0.5 * height);
    }

    void Accept(IWidgetRenderer& r) {
        r.Visit(this);
    }

    IWidget* WidgetAt(int x, int y) {
        if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height)
            return this;
        return NULL;
    }

    bool GetActive() {
        return active;
    }

    void SetActive(bool active) {
        this->active = active;
    }

    bool GetFocus() {
        return focus;
    }

    void SetFocus(bool focus) {
        this->focus = focus;
    }

    IWidget* FocusAt(int x, int y) {
        if (active) {
            Vector<2,float> v = Vector<2,float>(x,y) - center;
            if (v * v != 0) {
                v.Normalize();
                float dangle = 180.0f * acosf(v * focusVec) / Math::PI;
                float atanv1 = atan2f(v[1], v[0]);
                float atanv2 = atan2f(focusVec[1], focusVec[0]);
                if (atanv1 > atanv2) dangle = -dangle;
                focusVec = v;
                sweep += dangle;
                SetValue(GetValue() - step * dangle);
                if (sweep > 360) sweep -= 360;
                if (sweep < -360) sweep += 360;
            }
            return this;
        }
        if (WidgetAt(x,y)) {
            focusVec = Vector<2,float>(x,y) - center;
            if (focusVec * focusVec != 0) {
                SetFocus(true);
                return this;
            }
        }
        SetFocus(false);
        return NULL;
    }
    
    IWidget* ActivateAt(int x, int y) {
        if (WidgetAt(x,y)) {
            SetActive(true);
            return this;
        }
        SetActive(false);
        return NULL;
    }
    
    IWidget* ActivateFocus() {
        if (focus) {
            focusVec.Normalize();
            float atanv = atan2f(focusVec[1], focusVec[0]);
            startAngle = -180.0f * atanv / Math::PI;
            if (startAngle < 0) startAngle += 360;
            sweep = 0;
            SetActive(true);
            return this;
        }
        return NULL;
    }

    void Reset() {
        SetActive(false);
    }

    T GetValue() {
        return value;
    }

    void SetValue(T value) {
        this->value = value;
        e.Notify(ValueChangedEventArg<T>(value, this));
    }

    float GetStartAngle() {
        return startAngle;
    }

    float GetSweep() {
        return sweep;
    }
    

    // Math::Vector<2,int> GetPosition();
    // Math::Vector<2,int> GetDimensions();
    // void SetPosition(Math::Vector<2,int> pos);
    // void SetDimensions(Math::Vector<2,int> dim);
    // void Accept(IWidgetRenderer& r);
    // bool GetActive();
    // void SetActive(bool active);
    // bool GetFocus();
    // void SetFocus(bool focus);
    // IWidget* WidgetAt(int x, int y);
    // IWidget* FocusAt(int x, int y);
    // IWidget* ActivateAt(int x, int y);
    // IWidget* ActivateFocus();
    // void Reset();
    // T GetValue();
    // void SetValue(T value);
    // float GetStartAngle();
    // float GetSweep();
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_WIDGETS_CIRCULAR_SLIDER_
