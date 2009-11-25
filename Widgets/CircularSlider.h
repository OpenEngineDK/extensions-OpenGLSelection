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

#include <Widgets/ValueWidget.h>
#include <Widgets/IWidgetRenderer.h>
#include <Math/Math.h>
#include <Math/Vector.h>
#include <Core/Event.h>

namespace OpenEngine {
namespace Widgets {

using Math::Vector;

/**
 * On Screen Display Circular Slider Class.  
 *
 * A circular slider changes value in a predefined interval when
 * manipulated by the input device.
 *
 * @class CircularSlider CircularSlider.h OpenGLSelection/Widgets/CircularSlider.h
 */
template <class T>
class CircularSlider: public ValueWidget<T> {
private:
    float startAngle, sweep;
    Math::Vector<2,float> center, focusVec;
    T step;
public:
    CircularSlider(T value, T step)
        : ValueWidget<T>(value)
        , step(step)
    {}
    virtual ~CircularSlider() {}
    
    void SetPosition(Vector<2,int> pos) {
        IWidget::SetPosition(pos);
        center = Vector<2,float>(this->x + 0.5 * this->width, this->y + 0.5 * this->height);
    }

    void SetDimensions(Vector<2,int> dim) {
        IWidget::SetDimensions(dim);
        center = Vector<2,float>(this->x + 0.5 * dim[0], this->y + 0.5 * dim[1]);
    }

    void Accept(IWidgetRenderer& r) {
        r.Visit(this);
    }

    IWidget* WidgetAt(int x, int y) {
        if (x >= this->x && x < this->x + this->width && y >= this->y && y < this->y + this->height)
            return this;
        return NULL;
    }

    IWidget* FocusAt(int x, int y) {
        if (this->active) {
            Vector<2,float> v = Vector<2,float>(x,y) - center;
            if (v * v != 0) {
                v.Normalize();
                float dangle = 180.0f * acosf(v * focusVec) / Math::PI;
                float atanv1 = atan2f(v[1], v[0]);
                float atanv2 = atan2f(focusVec[1], focusVec[0]);
                if (atanv1 > atanv2) dangle = -dangle;
                focusVec = v;
                sweep += dangle;
                this->SetValue(this->GetValue() - this->step * dangle);
                if (sweep > 360) sweep -= 360;
                if (sweep < -360) sweep += 360;
            }
            return this;
        }
        if (WidgetAt(x,y)) {
            focusVec = Vector<2,float>(x,y) - center;
            if (focusVec * focusVec != 0) {
                this->SetFocus(true);
                return this;
            }
        }
        this->SetFocus(false);
        return NULL;
    }
    
    IWidget* ActivateAt(int x, int y) {
        if (WidgetAt(x,y)) {
            this->SetActive(true);
            return this;
        }
        this->SetActive(false);
        return NULL;
    }
    
    IWidget* ActivateFocus() {
        if (this->focus) {
            focusVec.Normalize();
            float atanv = atan2f(focusVec[1], focusVec[0]);
            startAngle = -180.0f * atanv / Math::PI;
            if (startAngle < 0) startAngle += 360;
            sweep = 0;
            this->SetActive(true);
            return this;
        }
        return NULL;
    }

    void Reset() {
        this->SetActive(false);
    }

    float GetStartAngle() {
        return startAngle;
    }

    float GetSweep() {
        return sweep;
    }
};

} // NS Widgets
} // NS OpenEngine
#endif //_OE_WIDGETS_CIRCULAR_SLIDER_
