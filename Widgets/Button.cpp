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
    : IWidget()
{
}

Button::~Button() {}
    
void Button::Accept(IWidgetRenderer& r) {
    r.Visit(this);
}

IWidget* Button::WidgetAt(int x, int y) {
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height)
        return this;
    return NULL;
}

void Button::SetActive(bool active) {
    this->active = active;
    e.Notify(StateChangedEventArg(active));
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

void Button::SetTextDimensions(Vector<2,int> dims) {
    textDims = dims;
    this->SetDimensions(dims);
}

} // NS Widgets
} // NS OpenEngine
