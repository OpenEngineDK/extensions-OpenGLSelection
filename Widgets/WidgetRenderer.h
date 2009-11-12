// On screen display - renderer.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_WIDGETS_WIDGET_RENDERER_
#define _OE_UTILS_WIDGETS_WIDGET_RENDERER_

#include <Widgets/IWidgetRenderer.h>
#include <Resources/ITextureResource.h>
#include <Resources/IFontResource.h>
#include <Math/Vector.h>


namespace OpenEngine {
    namespace Renderers {
        // class IWidgetRenderer;
        class TextureLoader;
    }
namespace Widgets {

class Button;
class Slider;
class CircularSlider;
class Collection;

/**
 * On Screen Display OpenGL Renderer Class.
 *
 * @class Renderer Renderer.h OpenGLSelection/Utils/Renderer.h
 */
class WidgetRenderer: public IWidgetRenderer {
private:
    Renderers::TextureLoader& texloader;
    Resources::ITextureResourcePtr sliderTex;
    Resources::IFontResourcePtr font;
    Resources::IFontResourcePtr smallfont;
    Math::Vector<4,float> activeColor, inactiveColor;
public:
    WidgetRenderer(Renderers::TextureLoader& texloader);
    virtual ~WidgetRenderer();

    void Render(Button& w);
    void Render(Slider& w);
    void Render(CircularSlider& w);
    void Render(Collection& w);
    
    Renderers::TextureLoader& GetTextureLoader();
    Resources::IFontResourcePtr GetFont();
    Resources::IFontResourcePtr GetSmallFont();
    // void SetRenderer(Renderers::IRenderer& r);
    // Renderers::IRenderer& GetRenderer();
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_WIDGETS_RENDERER_
