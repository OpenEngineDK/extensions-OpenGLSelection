// On screen display - circular slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/OSDCircularSlider.h>
#include <Utils/OSDIRenderer.h>
#include <Utils/OSDRenderer.h>
#include <Renderers/TextureLoader.h>

#include <Math/Math.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Utils {

using namespace Math;
using namespace Resources;
using namespace Renderers;

OSDCircularSlider::OSDCircularSlider(OSDRenderer& r)
    : texr(r.GetSmallFont()->CreateFontTexture())    
    , x(0)
    , y(0)
    , width(0)
    , height(0)
    , active(false)
    , focus(false)
    , value(0.0)
    , step(0.15)
{
    SetValue(0.0);
    r.GetTextureLoader().Load(texr, TextureLoader::RELOAD_IMMEDIATE);
}

OSDCircularSlider::~OSDCircularSlider() {}
    
Vector<2,int> OSDCircularSlider::GetPosition() {
    return Vector<2,int>(x, y);
}

Vector<2,int> OSDCircularSlider::GetDimensions() {
    return Vector<2,int>(width, height);
}

void OSDCircularSlider::SetPosition(Vector<2,int> pos) {
    x = pos[0];
    y = pos[1];
    center = Vector<2,float>(x + 0.5 * width, y + 0.5 * height);
}

void OSDCircularSlider::SetDimensions(Vector<2,int> dim) {
    width = dim[0];
    height = dim[1];
    center = Vector<2,float>(x + 0.5 * width, y + 0.5 * height);
}

void OSDCircularSlider::Accept(OSDIRenderer& r) {
    r.Render(*this);
}

OSDIWidget* OSDCircularSlider::WidgetAt(int x, int y) {
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height)
        return this;
    return NULL;
}

bool OSDCircularSlider::GetActive() {
    return active;
}

void OSDCircularSlider::SetActive(bool active) {
    this->active = active;
}

bool OSDCircularSlider::GetFocus() {
    return focus;
}

void OSDCircularSlider::SetFocus(bool focus) {
    this->focus = focus;
}

OSDIWidget* OSDCircularSlider::FocusAt(int x, int y) {
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

OSDIWidget* OSDCircularSlider::ActivateAt(int x, int y) {
    if (WidgetAt(x,y)) {
        SetActive(true);
        return this;
    }
    SetActive(false);
    return NULL;
}

OSDIWidget* OSDCircularSlider::ActivateFocus() {
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

void OSDCircularSlider::Reset() {
    SetActive(false);
}

float OSDCircularSlider::GetValue() {
    return value;
}

void OSDCircularSlider::SetValue(float value) {
    this->value = value;
    char s[10];
    sprintf(s, "%.1f", value);
    texr->SetText(s);
}

float OSDCircularSlider::GetStartAngle() {
    return startAngle;
}

float OSDCircularSlider::GetSweep() {
    return sweep;
}

ITextureResourcePtr OSDCircularSlider::GetTexture() {
    return texr;
}


int OSDCircularSlider::AngleToQuadrant(float angle) {
    if (angle >=   0 && angle <  90) return 1;
    if (angle >=  90 && angle < 180) return 2;
    if (angle >= 180 && angle < 270) return 3;
    if (angle >= 270 && angle < 360) return 4;
    return -1;
}

} // NS Utils
} // NS OpenEngine
