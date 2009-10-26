// On screen display - renderer.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_OSD_RENDERER_
#define _OE_UTILS_OSD_RENDERER_

#include <Utils/OSDIRenderer.h>
#include <Resources/ITextureResource.h>
#include <Resources/IFontResource.h>
#include <Math/Vector.h>


namespace OpenEngine {
    namespace Renderers {
        // class IRenderer;
        class TextureLoader;
    }
namespace Utils {

class OSDButton;
class OSDSlider;
class OSDCircularSlider;
class OSDCollection;

/**
 * On Screen Display OpenGL Renderer Class.
 *
 * @class OSDRenderer OSDRenderer.h OpenGLSelection/Utils/OSDRenderer.h
 */
class OSDRenderer: public OSDIRenderer {
private:
    Renderers::TextureLoader& texloader;
    Resources::ITextureResourcePtr sliderTex;
    Resources::IFontResourcePtr font;
    Resources::IFontResourcePtr smallfont;
    Math::Vector<4,float> activeColor, inactiveColor;
public:
    OSDRenderer(Renderers::TextureLoader& texloader);
    virtual ~OSDRenderer();

    void Render(OSDButton& w);
    void Render(OSDSlider& w);
    void Render(OSDCircularSlider& w);
    void Render(OSDCollection& w);
    
    Renderers::TextureLoader& GetTextureLoader();
    Resources::IFontResourcePtr GetFont();
    Resources::IFontResourcePtr GetSmallFont();
    // void SetRenderer(Renderers::IRenderer& r);
    // Renderers::IRenderer& GetRenderer();
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_OSD_RENDERER_
