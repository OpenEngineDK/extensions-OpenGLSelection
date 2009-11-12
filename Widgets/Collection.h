// On screen display - collection of widgets.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_WIDGETS_COLLECTION_
#define _OE_UTILS_WIDGETS_COLLECTION_

#include <Widgets/IWidget.h>
#include <list>

namespace OpenEngine {
namespace Widgets {

enum ResetMode {SIMPLE, RADIO, TOGGLE};
/**
 * On Screen Display - Collection of Widgets.  
 *
 * A button has positional info, can receive input events and can be
 * rendered. 
 *
 * @class Collection Collection.h OpenGLSelection/Utils/Collection.h
 */
class Collection: public IWidget {
private:
    ResetMode mode;
    std::list<IWidget*> widgets;
    int x, y, dx, dy, width, height;
    IWidget *focusWidget;
    bool active, focus;
public:
    explicit Collection(): mode(SIMPLE) {}
    Collection(ResetMode mode);
    virtual ~Collection();
    
    void UpdateWidgets();
    // IWidget
    Math::Vector<2,int> GetPosition();
    Math::Vector<2,int> GetDimensions();
    void SetPosition(Math::Vector<2,int> pos);
    void SetDimensions(Math::Vector<2,int> dim);
    void Accept(IWidgetRenderer& r);
    bool GetActive();
    void SetActive(bool active);
    bool GetFocus();
    void SetFocus(bool focus);
    IWidget* WidgetAt(int x, int y);
    IWidget* FocusAt(int x, int y);
    IWidget* ActivateAt(int x, int y);
    IWidget* ActivateFocus();
    void Reset();

    void SetupFonts(WidgetRenderer& r);

    // Collection
    void AddWidget(IWidget* w);
    void RemoveWidget(IWidget* w);
    std::list<IWidget*> GetWidgets();
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_WIDGETS_COLLECTION_
