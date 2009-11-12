// On screen display - renderer.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Widgets/WidgetRenderer.h>
#include <Widgets/Button.h>
#include <Widgets/Slider.h>
#include <Widgets/CircularSlider.h>
#include <Widgets/Collection.h>

#include <Resources/ResourceManager.h>
#include <Renderers/TextureLoader.h>
#include <Widgets/IWidgetRenderer.h>
#include <Meta/OpenGL.h>
#include <Math/Vector.h>

#include <list>

namespace OpenEngine {
namespace Widgets {

using namespace Math;
using namespace std;
using namespace Resources;
using namespace Renderers;

WidgetRenderer::WidgetRenderer(TextureLoader& texloader)
    : texloader(texloader)
    , activeColor(Vector<4,float>(.2,.2,.5, 1.0))//Vector<4,float>(0.9, .2, 0.2, 1.0f))
    , inactiveColor(Vector<4,float>(0.2, 0.2, 0.5, 0.5))
{
    sliderTex = ResourceManager<ITextureResource>::Create("slider_bg.png");
    texloader.Load(sliderTex);
    font = ResourceManager<IFontResource>::Create("Fonts/FreeSerif.ttf");
    font->SetPointSize(20);
    font->Load();
    smallfont = ResourceManager<IFontResource>::Create("Fonts/FreeSerif.ttf");
    smallfont->SetPointSize(14);
    smallfont->SetFontStyle(FONT_STYLE_BOLD);
    smallfont->Load();
}

WidgetRenderer::~WidgetRenderer() {}

TextureLoader& WidgetRenderer::GetTextureLoader() {
    return texloader;
}

IFontResourcePtr WidgetRenderer::GetFont() {
    return font;
}

IFontResourcePtr WidgetRenderer::GetSmallFont() {
    return smallfont;
}

void WidgetRenderer::Render(Button& w) {
    if (w.GetTexture() == NULL) {
        glBindTexture(GL_TEXTURE_2D, 0);
        CHECK_FOR_GL_ERROR();
    }
    else {
        glBindTexture(GL_TEXTURE_2D, w.GetTexture()->GetID());
        glEnable(GL_TEXTURE_2D);
        CHECK_FOR_GL_ERROR();
    }
    Vector<2,int> pos = w.GetPosition();
    Vector<2,int> dim(w.GetTexture()->GetWidth(), w.GetTexture()->GetHeight());
    float col[4];
    
    if (w.GetActive()) {
        pos = pos + Vector<2,int>(1,1);
        activeColor.ToArray(col); 
    }
    else {
        inactiveColor.ToArray(col);
        pos = pos - Vector<2,int>(1,1);
    }
    if (w.GetFocus()) {
    }
    else {}

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

void WidgetRenderer::Render(Slider& w) {
    glBindTexture(GL_TEXTURE_2D, sliderTex->GetID());
    glEnable(GL_TEXTURE_2D);

    Vector<2,int> pos = w.GetPosition();
    Vector<2,int> dim = w.GetDimensions();
    Vector<4,float> colr(.7,.0,.0,.9);
    
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

void WidgetRenderer::Render(CircularSlider& w) {
    Vector<2,int> pos = w.GetPosition();
    Vector<2,int> dim = w.GetDimensions();
    pos += Vector<2,int>(0.5 * dim[0], 0.5 * dim[1]);
    float colr[4];
    activeColor.ToArray(colr);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    Vector<2,int> dim_tex = Vector<2,int>(w.GetValueTexture()->GetWidth(),
                                          w.GetValueTexture()->GetHeight());
    Vector<2,int> pos_tex = pos - Vector<2,int>(0.5 * dim_tex[0], 0.5 * dim_tex[1]);
    if (w.GetValueTexture() == NULL) {
        glBindTexture(GL_TEXTURE_2D, 0);
        CHECK_FOR_GL_ERROR();
    }
    else {
        glBindTexture(GL_TEXTURE_2D, w.GetValueTexture()->GetID());
        glEnable(GL_TEXTURE_2D);
        CHECK_FOR_GL_ERROR();
    }
    glEnable(GL_TEXTURE_2D);
    //draw value
    glColor4f(colr[0], colr[1],colr[2],colr[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(pos_tex[0], pos_tex[1], -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(pos_tex[0], pos_tex[1] + dim_tex[1], -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(pos_tex[0] + dim_tex[0], pos_tex[1] + dim_tex[1], -1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(pos_tex[0] + dim_tex[0], pos_tex[1], -1.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    GLUquadric* q = gluNewQuadric();
    if (w.GetFocus()) {
        glPushMatrix();
        glTranslatef(pos[0], pos[1], -1);
        glColor4f(0.0, 0.0, 1.0, 0.1);
        gluDisk(q, 0.5*dim[0]*0.8, 0.5*dim[0], 20, 1);
        glPopMatrix();
    }
    if (w.GetActive()) {
        glPushMatrix();
        glTranslatef(pos[0], pos[1], -1);
        // glColor4f(0.2, 0.2, 1.0, .9);
        glColor4f(inactiveColor[0], inactiveColor[1], inactiveColor[2], inactiveColor[3]);
        gluPartialDisk(q, 0.5 * dim[0] * 0.8, 
                       0.5 * dim[0], 
                       20, 
                       1, 
                       90 + w.GetStartAngle(), 
                       w.GetSweep());
        glPopMatrix();
    }
    if (w.GetFocus()) {
        if (w.GetTextTexture() == NULL) {
            glBindTexture(GL_TEXTURE_2D, 0);
            CHECK_FOR_GL_ERROR();
        }
        else {
            glBindTexture(GL_TEXTURE_2D, w.GetTextTexture()->GetID());
            glEnable(GL_TEXTURE_2D);
            CHECK_FOR_GL_ERROR();
        }
        //draw value
        pos_tex[0] = w.GetPosition()[0] + 0.5*w.GetDimensions()[0] - 0.5*w.GetTextTexture()->GetWidth();
        pos_tex[1] = w.GetPosition()[1] + 0.9*w.GetDimensions()[1] - 0.5*w.GetTextTexture()->GetHeight();
        colr[3] = 0.6;
        glColor4fv(colr);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(pos_tex[0], pos_tex[1], -1.0);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(pos_tex[0], pos_tex[1] + w.GetTextTexture()->GetHeight(), -1.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(pos_tex[0] + w.GetTextTexture()->GetWidth(), 
                   pos_tex[1] + w.GetTextTexture()->GetHeight(), -1.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(pos_tex[0] + w.GetTextTexture()->GetWidth(), pos_tex[1], -1.0);
        glEnd();
    }
    gluDeleteQuadric(q);

}

void WidgetRenderer::Render(Collection& w) {
    //draw bg
    int pad = 10;
    Vector<2,int> pos = w.GetPosition();
    pos = Vector<2,int>(pos[0]-pad, pos[1]-pad); //padding
    Vector<2,int> dim = w.GetDimensions();
    dim = Vector<2,int>(dim[0]+2*pad, dim[1]+2*pad); 
    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    glColor4f(0.7,0.7,.9,0.9);
    glVertex3f(pos[0], pos[1], -1.0);
    glVertex3f(pos[0], pos[1] + dim[1], -1.0);
    glVertex3f(pos[0] + dim[0], pos[1] + dim[1], -1.0);
    glVertex3f(pos[0] + dim[0], pos[1], -1.0);
    glEnd();
    // border
    glLineWidth(4.0);
    glEnable(GL_BLEND);
    glBegin(GL_LINE_STRIP);
    // glColor4f(0.3, 0.3, 1.0, 0.8);
    glColor4f(activeColor[0], activeColor[1], activeColor[2], activeColor[3]);
    glVertex3f(pos[0], pos[1], -1.0);
    glVertex3f(pos[0], pos[1] + dim[1], -1.0);
    glVertex3f(pos[0] + dim[0], pos[1] + dim[1], -1.0);
    glVertex3f(pos[0] + dim[0], pos[1], -1.0);
    glVertex3f(pos[0], pos[1], -1.0);
    glEnd();
    list<IWidget*> widgets = w.GetWidgets();
    for (list<IWidget*>::iterator i = widgets.begin();
         i != widgets.end();
         i++) {
        (*i)->Accept(*this);
    }
}

} // NS Utils
} // NS OpenEngine
