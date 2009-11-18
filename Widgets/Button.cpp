// On screen display - button.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Widgets/Button.h>
#include <Widgets/IWidgetRenderer.h>

#include <Renderers/TextureLoader.h>

namespace OpenEngine {
namespace Widgets {

using namespace Math;
using namespace Resources;
using namespace Renderers;

Button::Button()
    : x(0)
    , y(0)
    , width(50)
    , height(30)
    , active(false)
    , focus(false) 
{
}

Button::~Button() {}
    
Vector<2,int> Button::GetPosition() {
    return Vector<2,int>(x, y);
}

Vector<2,int> Button::GetDimensions() {
    return Vector<2,int>(width, height);
}

void Button::SetPosition(Vector<2,int> pos) {
    x = pos[0];
    y = pos[1];
}

void Button::SetDimensions(Vector<2,int> dim) {
    width = dim[0];
    height = dim[1];
}

void Button::Accept(IWidgetRenderer& r) {
    r.Visit(this);
}

IWidget* Button::WidgetAt(int x, int y) {
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height)
        return this;
    return NULL;
}

bool Button::GetActive() {
    return active;
}

void Button::SetActive(bool active) {
    this->active = active;
    e.Notify(StateChangedEvent(active));
}

bool Button::GetFocus() {
    return focus;
}

void Button::SetFocus(bool focus) {
    this->focus = focus;
}

IWidget* Button::FocusAt(int x, int y) {
    if (WidgetAt(x,y)) {
        SetFocus(true);
        return this;
    }
    SetFocus(false);
    return NULL;
}

IWidget* Button::ActivateAt(int x, int y) {
    if (WidgetAt(x,y)) {
        SetActive(true);
        return this;
    }
    SetActive(false);
    return NULL;
}

IWidget* Button::ActivateFocus() {
    if (focus) {
        SetActive(true);
        return this;
    }
    return NULL;
}

void Button::Reset() {
    SetActive(false);
}

} // NS Widgets
} // NS OpenEngine
