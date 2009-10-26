// On screen display - button.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/OSDButton.h>
#include <Utils/OSDIRenderer.h>
#include <Utils/OSDRenderer.h>

#include <Renderers/TextureLoader.h>

namespace OpenEngine {
namespace Utils {

using namespace Math;
using namespace Resources;
using namespace Renderers;

OSDButton::OSDButton(OSDRenderer& r)
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

// OSDButton::OSDButton(ITextureResourcePtr texr): 
//     texr(texr)
//     , x(0)
//     , y(0)
//     , width(texr->GetWidth())
//     , height(texr->GetHeight())
//     , active(false)
//     , focus(false) {}

OSDButton::~OSDButton() {}
    
Vector<2,int> OSDButton::GetPosition() {
    return Vector<2,int>(x, y);
}

Vector<2,int> OSDButton::GetDimensions() {
    return Vector<2,int>(width, height);
}

void OSDButton::SetPosition(Vector<2,int> pos) {
    x = pos[0];
    y = pos[1];
}

void OSDButton::SetDimensions(Vector<2,int> dim) {
    width = dim[0];
    height = dim[1];
}

ITextureResourcePtr OSDButton::GetTexture() {
    return texr;
}

void OSDButton::SetCaption(string text) {
    texr->SetText(text);
    width = texr->GetWidth();
    height = texr->GetHeight();
}

string OSDButton::GetCaption() {
    return texr->GetText();
}


void OSDButton::Accept(OSDIRenderer& r) {
    r.Render(*this);
}

OSDIWidget* OSDButton::WidgetAt(int x, int y) {
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height)
        return this;
    return NULL;
}

bool OSDButton::GetActive() {
    return active;
}

void OSDButton::SetActive(bool active) {
    this->active = active;
}

bool OSDButton::GetFocus() {
    return focus;
}

void OSDButton::SetFocus(bool focus) {
    this->focus = focus;
}

OSDIWidget* OSDButton::FocusAt(int x, int y) {
    if (WidgetAt(x,y)) {
        SetFocus(true);
        return this;
    }
    SetFocus(false);
    return NULL;
}

OSDIWidget* OSDButton::ActivateAt(int x, int y) {
    if (WidgetAt(x,y)) {
        SetActive(true);
        return this;
    }
    SetActive(false);
    return NULL;
}

OSDIWidget* OSDButton::ActivateFocus() {
    if (focus) {
        SetActive(true);
        return this;
    }
    return NULL;
}

void OSDButton::Reset() {
    SetActive(false);
}

} // NS Utils
} // NS OpenEngine
