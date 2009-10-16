// On screen display - Widget interface, a widget has position and dimension.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_OSD_INTERFACE_WIDGET_
#define _OE_UTILS_OSD_INTERFACE_WIDGET_

#include <Math/Vector.h>

namespace OpenEngine {
namespace Utils {

class OSDIRenderer;
class PointingDevice;

/**
 * On Screen Display IWidget Interface.
 * 
 * @class OSDIWidget OSDIWidget.h OpenGLSelection/Utils/OSDIWidget.h
 */
class OSDIWidget {
public:
    virtual ~OSDIWidget() {};
    
    // positional methods
    virtual Math::Vector<2,int> GetPosition() = 0;
    virtual Math::Vector<2,int> GetDimensions() = 0;
    virtual void SetPosition(Math::Vector<2,int> pos) = 0;
    virtual void SetDimensions(Math::Vector<2,int> dim) = 0;
 
    // rendering (visitor callback) method
    virtual void Accept(OSDIRenderer& r) = 0;

    // input methods
    virtual OSDIWidget* WidgetAt(int x, int y) = 0;   // return widget at (x,y)
    virtual OSDIWidget* FocusAt(int x, int y) = 0;    // focus widget at (x,y)
    virtual OSDIWidget* ActivateAt(int x, int y) = 0; // activate widget at (x,y)
    virtual OSDIWidget* ActivateFocus() = 0;          // if focused then activate
    virtual void        Reset() = 0;                  // if focused then activate
    
    //state change methods
    virtual void SetActive(bool active) = 0;
    virtual bool GetActive() = 0;
    virtual void SetFocus(bool focus) = 0;
    virtual bool GetFocus() = 0;

};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_OSD_INTERFACE_WIDGET_
