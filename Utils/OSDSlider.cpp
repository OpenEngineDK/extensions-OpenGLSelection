// On screen display - slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/OSDSlider.h>
#include <Utils/OSDIRenderer.h>

namespace OpenEngine {
namespace Utils {

using namespace Math;
using namespace Resources;

OSDSlider::OSDSlider(): 
    x(0)
    , y(0)
    , width(0)
    , height(0)
    , active(false)
    , focus(false)
    , value(0.0)
{}

OSDSlider::~OSDSlider() {}
    
Vector<2,int> OSDSlider::GetPosition() {
    return Vector<2,int>(x, y);
}

Vector<2,int> OSDSlider::GetDimensions() {
    return Vector<2,int>(width, height);
}

void OSDSlider::SetPosition(Vector<2,int> pos) {
    x = pos[0];
    y = pos[1];
}

void OSDSlider::SetDimensions(Vector<2,int> dim) {
    width = dim[0];
    height = dim[1];
}

void OSDSlider::SetColor(Math::Vector<4,float> colr) {
    this->colr = colr;
}

Vector<4,float> OSDSlider::GetColor() {
    return colr;
}

void OSDSlider::Accept(OSDIRenderer& r) {
    r.Render(*this);
}

OSDIWidget* OSDSlider::WidgetAt(int x, int y) {
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height)
        return this;
    return NULL;
}

bool OSDSlider::GetActive() {
    return active;
}

void OSDSlider::SetActive(bool active) {
    this->active = active;
}

bool OSDSlider::GetFocus() {
    return focus;
}

void OSDSlider::SetFocus(bool focus) {
    this->focus = focus;
}

OSDIWidget* OSDSlider::FocusAt(int x, int y) {
    if (WidgetAt(x,y)) {
        if (active && width != 0) {
            SetValue(float(x - this->x) / float(width)); 
        }
        SetFocus(true);
        return this;
    }
    SetFocus(false);
    return NULL;
}

OSDIWidget* OSDSlider::ActivateAt(int x, int y) {
    if (WidgetAt(x,y)) {
        SetActive(true);
        return this;
    }
    SetActive(false);
    return NULL;
}

OSDIWidget* OSDSlider::ActivateFocus() {
    if (focus) {
        SetActive(true);
        return this;
    }
    return NULL;
}

void OSDSlider::Reset() {
    SetActive(false);
}

float OSDSlider::GetValue() {
    return value;
}

void OSDSlider::SetValue(float value) {
    this->value = value;
}


} // NS Utils
} // NS OpenEngine
