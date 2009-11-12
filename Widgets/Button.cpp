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
#include <Widgets/WidgetRenderer.h>

#include <Renderers/TextureLoader.h>

namespace OpenEngine {
namespace Widgets {

using namespace Math;
using namespace Resources;
using namespace Renderers;

Button::Button()
    : x(0)
    , y(0)
    , width(0)
    , height(0)
    , active(false)
    , focus(false) 
{
}
Button::Button(WidgetRenderer& r)
    : texr(r.GetFont()->CreateFontTexture())
    , x(0)
    , y(0)
    , width(0)
    , height(0)
    , active(false)
    , focus(false) 
{
    r.GetTextureLoader().Load(texr, TextureLoader::RELOAD_IMMEDIATE);
}

// Button::Button(ITextureResourcePtr texr): 
//     texr(texr)
//     , x(0)
//     , y(0)
//     , width(texr->GetWidth())
//     , height(texr->GetHeight())
//     , active(false)
//     , focus(false) {}

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

ITextureResourcePtr Button::GetTexture() {
    return texr;
}

void Button::SetCaption(string text) {
    caption = text;
    if (texr) { 
        texr->SetText(text);
        width = texr->GetWidth();
        height = texr->GetHeight();
    }
}

string Button::GetCaption() {
    return caption;
}


void Button::Accept(IWidgetRenderer& r) {
    r.Render(*this);
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

void Button::SetSmallFont(IFontResourcePtr font) {

}

void Button::SetLargeFont(IFontResourcePtr font) {

}

void Button::SetupFonts(WidgetRenderer& r) {
    texr = r.GetFont()->CreateFontTexture();
    texr->SetText(caption);
    width = texr->GetWidth();
    height = texr->GetHeight();
    r.GetTextureLoader().Load(texr, TextureLoader::RELOAD_IMMEDIATE);

}


} // NS Utils
} // NS OpenEngine
