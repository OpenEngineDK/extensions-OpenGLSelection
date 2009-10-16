// On screen display - collection of widgets.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_OSD_COLLECTION_
#define _OE_UTILS_OSD_COLLECTION_

#include <Utils/OSDIWidget.h>
#include <list>

namespace OpenEngine {
namespace Utils {

enum ResetMode {SIMPLE, RADIO};
/**
 * On Screen Display - Collection of Widgets.  
 *
 * A button has positional info, can receive input events and can be
 * rendered.  
 *
 * @class OSDCollection OSDCollection.h OpenGLSelection/Utils/OSDCollection.h
 */
class OSDCollection: public OSDIWidget {
private:
    ResetMode mode;
    std::list<OSDIWidget*> widgets;
    int x, y, width, height;
    OSDIWidget *focusWidget;
    void UpdateWidgets();
public:
    OSDCollection(ResetMode mode);
    virtual ~OSDCollection();
    
    // OSDIWidget
    Math::Vector<2,int> GetPosition();
    Math::Vector<2,int> GetDimensions();
    void SetPosition(Math::Vector<2,int> pos);
    void SetDimensions(Math::Vector<2,int> dim);
    void Accept(OSDIRenderer& r);
    bool GetActive();
    void SetActive(bool active);
    bool GetFocus();
    void SetFocus(bool focus);
    OSDIWidget* WidgetAt(int x, int y);
    OSDIWidget* FocusAt(int x, int y);
    OSDIWidget* ActivateAt(int x, int y);
    OSDIWidget* ActivateFocus();
    void Reset();

    // OSDCollection
    void AddWidget(OSDIWidget* w);
    void RemoveWidget(OSDIWidget* w);
    std::list<OSDIWidget*> GetWidgets();
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_OSD_COLLECTION_
