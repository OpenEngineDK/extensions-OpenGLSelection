// On screen display - collection of widgets.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WIDGETS_COLLECTION_
#define _OE_WIDGETS_COLLECTION_

#include <Widgets/IWidget.h>
#include <Core/IListener.h>
#include <list>

namespace OpenEngine {
namespace Widgets {

using Core::IListener;
/**
 * On Screen Display - Collection of Widgets.  
 *
 * A button has positional info, can receive input events and can be
 * rendered. 
 *
 * @class Collection Collection.h OpenGLSelection/Utils/Collection.h
 */
class Collection: public IWidget, public IListener<DimensionsChangedEventArg> {
public:
enum Mode {SIMPLE, RADIO, TOGGLE};
private:
    Mode mode;
    std::list<IWidget*> widgets;
    int dx, dy;
    bool fixed, bg;
    IWidget *focusWidget;
    void UpdateWidgets();
    // layout variables
    int spacing;
    Vector<4,int> padding;
public:
    explicit Collection();
    Collection(Mode mode);
    virtual ~Collection();
    // IWidget
    void SetPosition(Math::Vector<2,int> pos);
    void Accept(IWidgetRenderer& r);
    IWidget* WidgetAt(int x, int y);
    IWidget* FocusAt(int x, int y);
    IWidget* ActivateAt(int x, int y);
    IWidget* ActivateFocus();
    void Reset();

    // Collection
    void Handle(DimensionsChangedEventArg e);

    // Widgets
    void AddWidget(IWidget* w);
    void RemoveWidget(IWidget* w);
    std::list<IWidget*> GetWidgets();

    //Layout
    void SetSpacing(int spacing);
    int GetSpacing();
    void SetPadding(Vector<4,int> padding);
    Vector<4,int> GetPadding();
    void SetFixed(bool fixed);
    bool GetFixed();
    void SetBackground(bool bg);
    bool GetBackground();
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_WIDGETS_COLLECTION_
