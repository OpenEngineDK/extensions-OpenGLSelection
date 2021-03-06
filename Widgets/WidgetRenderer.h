// On screen display - renderer.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WIDGETS_WIDGET_RENDERER_
#define _OE_WIDGETS_WIDGET_RENDERER_

#include <Widgets/IWidget.h>
#include <Widgets/ValueWidget.h>
#include <Widgets/IWidgetRenderer.h>
#include <Resources/ITexture2D.h>
#include <Resources/IFontTextureResource.h>
#include <Resources/IFontResource.h>
#include <Math/Vector.h>
#include <Core/IListener.h>
#include <map>
#include <list>

namespace OpenEngine {
    namespace Renderers {
        class TextureLoader;
    }
namespace Widgets {

using Core::IListener;
using Resources::IFontTextureResourcePtr;
using Resources::IFontResourcePtr;
using Resources::ITexture2DPtr;
using Renderers::TextureLoader;
using std::map;
using std::list;

class Button;
class Slider;
class Collection;
template <class T>
class CircularSlider;

    class FTPair {
    public: 
        IFontTextureResourcePtr tex;
        IFontResourcePtr font;
        FTPair( IFontTextureResourcePtr tex,
                IFontResourcePtr font): tex(tex), font(font) {}
        virtual ~FTPair() {}
    };

/**
 * On Screen Display OpenGL Renderer Class.
 *
 * @class Renderer Renderer.h OpenGLSelection/Utils/Renderer.h
 */
class WidgetRenderer: public IWidgetRenderer {
private:

    class Initializer
        : public IWidgetRenderer
        , IListener<TextChangedEventArg>
        , IListener<ValueChangedEventArg<float> >
        , IListener<ValueChangedEventArg<int> > {
    private:
        TextureLoader& texloader;
        IFontResourcePtr largefont;
        IFontResourcePtr smallfont;
        map<IWidget*, FTPair*>& text_map;
        map<IWidget*, FTPair*>& val_map;
        inline ITexture2DPtr LookupText(IWidget* w);
        inline FTPair* RenderText(string s, IFontResourcePtr font, FTPair* ftp);
    public:
        Initializer(TextureLoader& texloader,         
                    map<IWidget*, FTPair*>& text_map,
                    map<IWidget*, FTPair*>& val_map);
        void Visit(Button* w);
        void Visit(Slider* w);
        void Visit(CircularSlider<float>* w);
        void Visit(CircularSlider<int>* w);
        void Visit(Collection* w);
        void Handle(TextChangedEventArg e);
        void Handle(ValueChangedEventArg<float> e);
        void Handle(ValueChangedEventArg<int> e);
    } initializer;

    list<IWidget*> widgets;
    //maps to hold widget font textures
    map<IWidget*, FTPair*> text_map;
    map<IWidget*, FTPair*> val_map;
    ITexture2DPtr sliderTex;
    Math::Vector<4,float> activeColor, inactiveColor, bgColor;
    float alpha;
    int collDepth;
    inline void RenderQuad(ITexture2DPtr texr, float x, float y, float width, float height, float* col);
    void RenderCircularSlider(IWidget* w, float angle, float sweep);
public:
    WidgetRenderer(Renderers::TextureLoader& texloader);
    virtual ~WidgetRenderer();

    void Visit(Button* w);
    void Visit(Slider* w);
    void Visit(CircularSlider<float>* w);
    void Visit(CircularSlider<int>* w);
    void Visit(Collection* w);

    void RenderWidgets();
    void AddWidget(IWidget* w);
};

} // NS Widgets
} // NS OpenEngine
#endif //_OE_UTILS_WIDGETS_RENDERER_
