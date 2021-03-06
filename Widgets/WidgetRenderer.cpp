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
#include <Meta/OpenGL.h>

#include <list>

namespace OpenEngine {
namespace Widgets {

using namespace Math;
using namespace std;
using namespace Resources;
using namespace Renderers;

WidgetRenderer::WidgetRenderer(TextureLoader& texloader)
    : initializer(texloader, text_map, val_map)
    , activeColor(Vector<4,float>(.2,.2,.5, 1.0))
    , inactiveColor(Vector<4,float>(0.2, 0.2, 0.5, 0.5))
    , bgColor(Vector<4,float>(0.7,0.7,.9,0.9))
    , alpha(1.0)
    , collDepth(-1)
{
    sliderTex = ResourceManager<ITexture2D>::Create("slider_bg.png");
    texloader.Load(sliderTex);
}

WidgetRenderer::~WidgetRenderer() {}

void WidgetRenderer::AddWidget(IWidget* w) {
    widgets.push_back(w);
    w->Accept(initializer);
}

void WidgetRenderer::RenderWidgets() {
    glPushAttrib(GL_ENABLE_BIT);
    glPushAttrib(GL_DEPTH_BUFFER_BIT);
    glPushAttrib(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (list<IWidget*>::iterator itr = widgets.begin();
         itr != widgets.end();
         ++itr) {
        (*itr)->Accept(*this);
    }
    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
}

void WidgetRenderer::RenderQuad(ITexture2DPtr texr, 
                                float x, 
                                float y, 
                                float width, 
                                float height, 
                                float* col) 
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texr->GetID());
    glBegin(GL_QUADS);
    glColor4fv(col);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(x, y, -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(x, y + height, -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(x + width, y + height, -1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(x + width, y, -1.0);
    glEnd();    
    glDisable(GL_TEXTURE_2D);
}

void WidgetRenderer::Visit(Button* w) {
    ITexture2DPtr text = text_map[w]->tex;
    Vector<2,int> pos = w->GetPosition();
    Vector<2,int> dim(text->GetWidth(), text->GetHeight());
    float col[4];
    if (w->GetActive()) {
        pos = pos + Vector<2,int>(1,1);
        activeColor.ToArray(col); 
    }
    else {
        inactiveColor.ToArray(col);
        pos = pos - Vector<2,int>(1,1);
    }
    col[3] *= alpha;
    RenderQuad(text, pos[0], pos[1], dim[0], dim[1], col);
}

void WidgetRenderer::Visit(Slider* w) {
    ITexture2DPtr text = text_map[w]->tex; 
    Vector<2,int> pos = w->GetPosition();
    Vector<2,int> dim = w->GetDimensions();
    
    float val = w->GetValue();
    float knob_width = 0.5 * dim[1];
    float knob_height = dim[1]; 
    Vector<2,float> knob_pos(pos[0] + dim[0] * val - (0.5 * knob_width),
                             pos[1]);

    dim[1] *= 0.5; 
    pos[1] += 0.5 * dim[1]; 

    float col[4];
    activeColor.ToArray(col);
    col[3] *= alpha;
    //draw slider
    RenderQuad(sliderTex, pos[0], pos[1], dim[0], dim[1], col);
    //draw knob
    RenderQuad(sliderTex, knob_pos[0], knob_pos[1], knob_width, knob_height, col);
    //draw knob
    RenderQuad(text, pos[0], pos[1] + dim[1] * 0.5, text->GetWidth(), text->GetHeight(), col);
}

void WidgetRenderer::RenderCircularSlider(IWidget* w, float angle, float sweep) {
    ITexture2DPtr text = text_map[w]->tex; 
    IFontTextureResourcePtr val  = val_map[w]->tex;
    Vector<2,int> pos = w->GetPosition();
    Vector<2,int> dim = w->GetDimensions();
    pos += Vector<2,int>(0.5 * dim[0], 0.5 * dim[1]);
    float col[4];
    activeColor.ToArray(col);
    Vector<2,int> dim_tex = Vector<2,int>(val->GetWidth(),
                                          val->GetHeight());
    Vector<2,int> pos_tex = pos - Vector<2,int>(0.5 * dim_tex[0], 0.5 * dim_tex[1]);
    //draw value
    RenderQuad(val, pos_tex[0], pos_tex[1], dim_tex[0], dim_tex[1], col);
    GLUquadric* q = gluNewQuadric();
    if (w->GetFocus()) {
        glPushMatrix();
        glTranslatef(pos[0], pos[1], -1);
        glColor4f(0.0, 0.0, 1.0, 0.1);
        gluDisk(q, 0.5*dim[0]*0.8, 0.5*dim[0], 20, 1);
        glPopMatrix();
    }
    if (w->GetActive()) {
        glPushMatrix();
        glTranslatef(pos[0], pos[1], -1);
        // glColor4f(0.2, 0.2, 1.0, .9);
        glColor4f(inactiveColor[0], inactiveColor[1], inactiveColor[2], inactiveColor[3]);
        gluPartialDisk(q, 0.5 * dim[0] * 0.8, 
                       0.5 * dim[0], 
                       20, 
                       1, 
                       90 + angle, 
                       sweep);
        glPopMatrix();
    }
    if (w->GetFocus()) {
        //draw text
        pos_tex[0] = w->GetPosition()[0] + 0.5*w->GetDimensions()[0] - 0.5*text->GetWidth();
        pos_tex[1] = w->GetPosition()[1] + 0.9*w->GetDimensions()[1] - 0.5*text->GetHeight();
        col[3] = 0.6;
        RenderQuad(text, pos_tex[0], pos_tex[1], text->GetWidth(), text->GetHeight(), col);
    }
    gluDeleteQuadric(q);
}

void WidgetRenderer::Visit(CircularSlider<float>* w) {
    RenderCircularSlider(w, w->GetStartAngle(), w->GetSweep());
}

void WidgetRenderer::Visit(CircularSlider<int>* w) {
    RenderCircularSlider(w, w->GetStartAngle(), w->GetSweep());
}

void WidgetRenderer::Visit(Collection* w) {
    if (!w->GetVisible()) return;
    if (++collDepth == 0) {
        if (!w->GetFocus()) alpha = 0.7;
        else alpha = 1.0;
    }
    if (w->GetBackground()) {
        //draw bg
        Vector<2,int> pos = w->GetPosition();
        Vector<2,int> dim = w->GetDimensions();
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        float col[4];
        bgColor.ToArray(col);
        col[3] *= alpha;
        glColor4fv(col);
        glVertex3f(pos[0], pos[1], -1.0);
        glVertex3f(pos[0], pos[1] + dim[1], -1.0);
        glVertex3f(pos[0] + dim[0], pos[1] + dim[1], -1.0);
        glVertex3f(pos[0] + dim[0], pos[1], -1.0);
        glEnd();
        // border
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(2.0);
        activeColor.ToArray(col);
        col[3] *= alpha;
        glBegin(GL_LINE_STRIP);
        glColor4fv(col);
        glVertex3f(pos[0], pos[1], -1.0);
        glVertex3f(pos[0], pos[1] + dim[1], -1.0);
        glVertex3f(pos[0] + dim[0], pos[1] + dim[1], -1.0);
        glVertex3f(pos[0] + dim[0], pos[1], -1.0);
        glVertex3f(pos[0], pos[1], -1.0);
        glEnd();
    }
    
    list<IWidget*> widgets = w->GetWidgets();
    for (list<IWidget*>::iterator i = widgets.begin();
         i != widgets.end();
         i++) {
        (*i)->Accept(*this);
    }
    --collDepth;
}


// initializer

WidgetRenderer::Initializer::Initializer(TextureLoader& texloader,
                                         map<IWidget*,FTPair*>& text_map,
                                         map<IWidget*,FTPair*>& val_map)
    : texloader(texloader)
    , text_map(text_map)
    , val_map(val_map) 
{
    largefont = ResourceManager<IFontResource>::Create("Fonts/FreeSerif.ttf");
    largefont->SetSize(16);
    largefont->Load();
    smallfont = ResourceManager<IFontResource>::Create("Fonts/FreeSerif.ttf");
    smallfont->SetSize(12);
    smallfont->SetStyle(FONT_STYLE_BOLD);
    smallfont->Load();
}

void WidgetRenderer::Initializer::Visit(Button* w) {
    w->TextChangedEvent().Attach(*this);
    FTPair* pt = RenderText(w->GetText(), largefont, text_map[w]);
    text_map[w] = pt;
    texloader.Load(pt->tex, TextureLoader::RELOAD_IMMEDIATE);
    w->SetTextDimensions(Vector<2,int>(pt->tex->GetWidth(),pt->tex->GetHeight()));        
 }

void WidgetRenderer::Initializer::Visit(Slider* w) {
    w->TextChangedEvent().Attach(*this);
    FTPair* pt = RenderText(w->GetText(), smallfont, text_map[w]);
    text_map[w] = pt;
    texloader.Load(pt->tex, TextureLoader::RELOAD_IMMEDIATE);
    w->SetTextDimensions(Vector<2,int>(pt->tex->GetWidth(),pt->tex->GetHeight()));        
}

void WidgetRenderer::Initializer::Visit(CircularSlider<float>* w) {
    w->ValueChangedEvent().Attach(*this);
    w->TextChangedEvent().Attach(*this);
    FTPair* pt = RenderText(w->GetText(), smallfont, text_map[w]);
    text_map[w] = pt;
    char s[10];
    sprintf(s, "%.1f", w->GetValue());
    FTPair* pv = RenderText(string(s), smallfont, val_map[w]);
    val_map[w] = pv;
    texloader.Load(pt->tex, TextureLoader::RELOAD_IMMEDIATE);
    texloader.Load(pv->tex, TextureLoader::RELOAD_IMMEDIATE);
    w->SetTextDimensions(Vector<2,int>(pt->tex->GetWidth(),pt->tex->GetHeight()));        
}

void WidgetRenderer::Initializer::Visit(CircularSlider<int>* w) {
    w->ValueChangedEvent().Attach(*this);
    w->TextChangedEvent().Attach(*this);
    FTPair* pt = RenderText(w->GetText(), smallfont, text_map[w]);
    text_map[w] = pt;
    char s[10];
    sprintf(s, "%d", w->GetValue());
    FTPair* pv = RenderText(string(s), smallfont, val_map[w]);
    val_map[w] = pv;
    texloader.Load(pt->tex, TextureLoader::RELOAD_IMMEDIATE);
    texloader.Load(pv->tex, TextureLoader::RELOAD_IMMEDIATE);
    w->SetTextDimensions(Vector<2,int>(pt->tex->GetWidth(),pt->tex->GetHeight()));        
}

void WidgetRenderer::Initializer::Visit(Collection* w) {
    list<IWidget*> widgets = w->GetWidgets();
    for (list<IWidget*>::iterator i = widgets.begin();
         i != widgets.end();
         i++) {
        (*i)->Accept(*this);
    }
}

FTPair* WidgetRenderer::Initializer::RenderText(string s, IFontResourcePtr font, FTPair* ftp) {
    FTPair* p;
    if (ftp) {
        p = ftp;
        
    }
    else {
        Vector<2,int> dim = font->TextDim(s);
        IFontTextureResourcePtr texr = font->CreateFontTexture(dim[0],dim[1]);    
        p = new FTPair(texr, font);
    }
    p->tex->Clear(Vector<4,float> (1,1,1,0));
    font->RenderText(s, p->tex, 0, 0);
    return p;
}

ITexture2DPtr WidgetRenderer::Initializer::LookupText(IWidget* w) {
    FTPair* p = text_map[w];
    if (p == NULL) {
        w->Accept(*this);
    }
    p = text_map[w];
    return p->tex;
}

void WidgetRenderer::Initializer::Handle(TextChangedEventArg e) {
    FTPair* p = text_map[e.widget];
    if (!p) {
        RenderText(e.text, p->font, p);
        e.widget->SetTextDimensions(Vector<2,int>(p->tex->GetWidth(),p->tex->GetHeight()));
    }
}

void WidgetRenderer::Initializer::Handle(ValueChangedEventArg<float> e) {
    FTPair* p = val_map[e.widget];
    if (!p) return;
    char s[10];
    snprintf(s, 10, "%.1f", e.value);
    RenderText(string(s), p->font, p);
}

void WidgetRenderer::Initializer::Handle(ValueChangedEventArg<int> e) {
    FTPair* p = val_map[e.widget];
    if (!p) return;
    char s[10];
    snprintf(s, 10, "%d", e.value);
    RenderText(string(s), p->font, p);
}

} // NS Utils
} // NS OpenEngine
