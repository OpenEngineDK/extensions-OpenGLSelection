// On screen display - button.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_OSD_BUTTON_
#define _OE_UTILS_OSD_BUTTON_

#include <Utils/OSDIWidget.h>

#include <Math/Vector.h>
#include <Resources/IFontTextureResource.h>

#include <string.h>

namespace OpenEngine {
namespace Utils {

using std::string;

class OSDRenderer;

/**
 * On Screen Display Button Class.  
 *
 * A button has positional info, can receive input events and can be
 * rendered.  
 *
 * @class OSDButton OSDButton.h OpenGLSelection/Utils/OSDButton.h
 */
class OSDButton: public OSDIWidget {
private:
    Resources::IFontTextureResourcePtr texr;
    Math::Vector<4,float> colr, activeColr;
    int x, y, width, height;
    bool active, focus;
public:
    OSDButton(OSDRenderer& r);
    //OSDButton(Resources::ITextureResourcePtr texr);
    virtual ~OSDButton();
    
    Math::Vector<2,int> GetPosition();
    Math::Vector<2,int> GetDimensions();
    void SetPosition(Math::Vector<2,int> pos);
    void SetDimensions(Math::Vector<2,int> dim);
    void Accept(OSDIRenderer& r);
    bool GetActive();
    void SetActive(bool active);
    bool GetFocus();
    void SetFocus(bool focus);
    void SetCaption(string text);
    string GetCaption();

    OSDIWidget* WidgetAt(int x, int y);
    OSDIWidget* FocusAt(int x, int y);
    OSDIWidget* ActivateAt(int x, int y);
    OSDIWidget* ActivateFocus();
    void Reset();

    // Math::Vector<4,float> GetColor();
    // Math::Vector<4,float> GetActiveColor();
    Resources::ITextureResourcePtr GetTexture();
    // void SetColor(Math::Vector<4,float> colr);
    // void SetActiveColor(Math::Vector<4,float> colr);
    // void SetTexture(Resources::ITextureResourcePtr texr);
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_OSD_BUTTON_
