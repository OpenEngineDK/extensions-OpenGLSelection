// On screen display - circular slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Widgets/CircularSlider.h>
#include <Widgets/IWidgetRenderer.h>
#include <Math/Math.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Widgets {

using namespace Math;
using namespace Resources;
using namespace Renderers;


CircularSlider::~CircularSlider() {}
    
Vector<2,int> CircularSlider::GetPosition() {
    return Vector<2,int>(x, y);
}

Vector<2,int> CircularSlider::GetDimensions() {
    return Vector<2,int>(width, height);
}

void CircularSlider::SetPosition(Vector<2,int> pos) {
    x = pos[0];
    y = pos[1];
    center = Vector<2,float>(x + 0.5 * width, y + 0.5 * height);
}

void CircularSlider::SetDimensions(Vector<2,int> dim) {
    width = dim[0];
    height = dim[1];
    center = Vector<2,float>(x + 0.5 * width, y + 0.5 * height);
}

void CircularSlider::Accept(IWidgetRenderer& r) {
    r.Visit(this);
}

IWidget* CircularSlider::WidgetAt(int x, int y) {
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height)
        return this;
    return NULL;
}

bool CircularSlider::GetActive() {
    return active;
}

void CircularSlider::SetActive(bool active) {
    this->active = active;
}

bool CircularSlider::GetFocus() {
    return focus;
}

void CircularSlider::SetFocus(bool focus) {
    this->focus = focus;
}

IWidget* CircularSlider::FocusAt(int x, int y) {
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

IWidget* CircularSlider::ActivateAt(int x, int y) {
    if (WidgetAt(x,y)) {
        SetActive(true);
        return this;
    }
    SetActive(false);
    return NULL;
}

IWidget* CircularSlider::ActivateFocus() {
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

void CircularSlider::Reset() {
    SetActive(false);
}

T CircularSlider::GetValue() {
    return value;
}

void CircularSlider::SetValue(T value) {
    this->value = value;
    // char s[10];
    // sprintf(s, "%.1f", value);
    // if (val_texr) { 
    //     val_texr->SetText(s);
    // }
    e.Notify(ValueChangedEventArg<T>(value, this));
}

float CircularSlider::GetStartAngle() {
    return startAngle;
}

float CircularSlider::GetSweep() {
    return sweep;
}

} // NS Widgets
} // NS OpenEngine
