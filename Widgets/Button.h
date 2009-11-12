// On screen display - button.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_WIDGETS_BUTTON_
#define _OE_UTILS_WIDGETS_BUTTON_

#include <Widgets/IWidget.h>
#include <Core/Event.h>

#include <Math/Vector.h>
#include <Resources/IFontTextureResource.h>

#include <string.h>

namespace OpenEngine {
namespace Widgets {

using std::string;
using Core::Event;

class WidgetRenderer;

class StateChangedEvent{
public:
    bool state;
    StateChangedEvent(bool state): state(state) {}
};

/**
 * On Screen Display Button Class.  
 *
 * A button has positional info, can receive input events and can be
 * rendered.  
 *
 * @class Button Button.h OpenGLSelection/Utils/Button.h
 */
class Button: public IWidget {
private:
    Resources::IFontTextureResourcePtr texr;
    Math::Vector<4,float> colr, activeColr;
    int x, y, width, height;
    bool active, focus;
    Event<StateChangedEvent> e;
    string caption;
public:
    Event<StateChangedEvent>& ChangedEvent() {return e;}

    void SetSmallFont(IFontResourcePtr font);
    void SetLargeFont(IFontResourcePtr font);
    void SetupFonts(WidgetRenderer& r);

    explicit Button();
    Button(WidgetRenderer& r);
    //Button(Resources::ITextureResourcePtr texr);
    virtual ~Button();
    
    Math::Vector<2,int> GetPosition();
    Math::Vector<2,int> GetDimensions();
    void SetPosition(Math::Vector<2,int> pos);
    void SetDimensions(Math::Vector<2,int> dim);
    void Accept(IWidgetRenderer& r);
    bool GetActive();
    void SetActive(bool active);
    bool GetFocus();
    void SetFocus(bool focus);
    void SetCaption(string text);
    string GetCaption();

    IWidget* WidgetAt(int x, int y);
    IWidget* FocusAt(int x, int y);
    IWidget* ActivateAt(int x, int y);
    IWidget* ActivateFocus();
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
#endif //_OE_UTILS_WIDGETS_BUTTON_
