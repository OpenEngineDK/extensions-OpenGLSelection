// On screen display - slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Widgets/Slider.h>
#include <Widgets/IWidgetRenderer.h>

namespace OpenEngine {
namespace Widgets {

using namespace Math;

Slider::Slider()
    : ValueWidget<float> (0.0) 
{}

Slider::~Slider() {}
    
void Slider::Accept(IWidgetRenderer& r) {
    r.Visit(this);
}

IWidget* Slider::WidgetAt(int x, int y) {
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height)
        return this;
    return NULL;
}

IWidget* Slider::FocusAt(int x, int y) {
    if (active && width != 0) {
        SetValue(float(x - this->x) / float(width)); 
    }
    if (WidgetAt(x,y)) {
        SetFocus(true);
        return this;
    }
    SetFocus(false);
    return NULL;
}

IWidget* Slider::ActivateAt(int x, int y) {
    if (WidgetAt(x,y)) {
        SetActive(true);
        return this;
    }
    SetActive(false);
    return NULL;
}

IWidget* Slider::ActivateFocus() {
    if (focus) {
        SetActive(true);
        return this;
    }
    return NULL;
}

void Slider::Reset() {
    SetActive(false);
}

float Slider::GetValue() {
    return value;
}

void Slider::SetValue(float value) {
    value = fmin(1.0, value); 
    value = fmax(0.0, value); 
    ValueWidget<float>::SetValue(value);
}

} // NS Utils
} // NS OpenEngine
