// On screen display - renderer.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/OSDRenderer.h>
#include <Utils/OSDButton.h>
#include <Utils/OSDSlider.h>
#include <Utils/OSDCollection.h>

#include <Resources/ResourceManager.h>
#include <Renderers/TextureLoader.h>
#include <Renderers/IRenderer.h>
#include <Meta/OpenGL.h>
#include <Math/Vector.h>

#include <list>

namespace OpenEngine {
namespace Utils {

using namespace Math;
using namespace std;
using namespace Resources;
using namespace Renderers;

OSDRenderer::OSDRenderer(TextureLoader& texloader)
    : texloader(texloader)
    , activeColor(Vector<4,float>(0.9, .2, 0.2, 1.0f))
    , inactiveColor(Vector<4,float>(0.0, 0.0, 0.0, 1.0))
{
    sliderTex = ResourceManager<ITextureResource>::Create("slider_bg.png");
    texloader.Load(sliderTex);
    font = ResourceManager<IFontResource>::Create("Fonts/FreeMono.ttf");
    font->SetPointSize(20);
    font->Load();
}

OSDRenderer::~OSDRenderer() {}

TextureLoader& OSDRenderer::GetTextureLoader() {
    return texloader;
}

// void OSDRenderer::SetRenderer(IRenderer& r) {
//     this->r = r;
// }

// IRenderer& OSDRenderer::GetRenderer() {
//     return r;
// }

IFontResourcePtr OSDRenderer::GetFont() {
    return font;
}

void OSDRenderer::Render(OSDButton& w) {
    if (w.GetTexture() == NULL) {
        glBindTexture(GL_TEXTURE_2D, 0);
        CHECK_FOR_GL_ERROR();
    }
    else {
        glBindTexture(GL_TEXTURE_2D, w.GetTexture()->GetID());
        glEnable(GL_TEXTURE_2D);
        CHECK_FOR_GL_ERROR();
    }
    Vector<2,int>   pos = w.GetPosition();
    Vector<2,int>   dim(w.GetTexture()->GetWidth(), w.GetTexture()->GetHeight());// = w.GetDimensions();
    float col[4];
    
    if (w.GetActive()) 
        activeColor.ToArray(col);
    else 
        inactiveColor.ToArray(col);
    
    if (w.GetFocus()) {
        pos = pos - Vector<2,int>(3,3);
        dim = dim + Vector<2,int>(1,1);
    }

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4fv(col);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(pos[0], pos[1], -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(pos[0], pos[1] + dim[1], -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(pos[0] + dim[0], pos[1] + dim[1], -1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(pos[0] + dim[0], pos[1], -1.0);
    glEnd();    
}

void OSDRenderer::Render(OSDSlider& w) {
    glBindTexture(GL_TEXTURE_2D, sliderTex->GetID());
    glEnable(GL_TEXTURE_2D);

    Vector<2,int> pos = w.GetPosition();
    Vector<2,int> dim = w.GetDimensions();
    Vector<4,float> colr = w.GetColor();    
    
    float val = w.GetValue();
    float knob_width = 10.0f;
    float knob_height = dim[1] + 10.0f; 
    Vector<2,float> knob_pos(pos[0] + dim[0] * val - (0.5 * knob_width),
                             pos[1] + 0.5 * dim[1] - 0.5 * knob_height);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(colr[0], colr[1], colr[2], colr[3]);
    //draw slider
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(pos[0], pos[1], -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(pos[0], pos[1] + dim[1], -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(pos[0] + dim[0], pos[1] + dim[1], -1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(pos[0] + dim[0], pos[1], -1.0);
    //draw knob
    glTexCoord2f(0.0, 0.0);
    glVertex3f(knob_pos[0], knob_pos[1], -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(knob_pos[0], knob_pos[1] + knob_height, -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(knob_pos[0] + knob_width, knob_pos[1] + knob_height, -1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(knob_pos[0] + knob_width, knob_pos[1], -1.0);
    glEnd();    
}

void OSDRenderer::Render(OSDCollection& w) {
    list<OSDIWidget*> widgets = w.GetWidgets();
    for (list<OSDIWidget*>::iterator i = widgets.begin();
         i != widgets.end();
         i++) {
        (*i)->Accept(*this);
    }
}

} // NS Utils
} // NS OpenEngine
