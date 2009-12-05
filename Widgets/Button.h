// On screen display - button.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WIDGETS_BUTTON_
#define _OE_WIDGETS_BUTTON_

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

class StateChangedEventArg {
public:
    bool state;
    StateChangedEventArg (bool state): state(state) {}
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
    Event<StateChangedEventArg> e;
public:
    Button();
    virtual ~Button();
    
    void SetActive(bool active);
    void Accept(IWidgetRenderer& r);
    IWidget* WidgetAt(int x, int y);
    IWidget* FocusAt(int x, int y);
    IWidget* ActivateAt(int x, int y);
    IWidget* ActivateFocus();
    void Reset();
    void SetTextDimensions(Vector<2,int> dims);
    Event<StateChangedEventArg>& StateChangedEvent() { return e; }

};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_WIDGETS_BUTTON_
