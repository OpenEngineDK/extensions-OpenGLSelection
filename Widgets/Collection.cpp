// On screen display - collection of widgets.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Widgets/Collection.h>
#include <Widgets/IWidgetRenderer.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Widgets {

using namespace Math;
using namespace std;

Collection::Collection(ResetMode mode)
    : mode(mode)
    , x(0) 
    , y(0) 
    , width(0) 
    , height(0)
    , focusWidget(NULL)
    , active(false)
    , focus(false)
{}

Collection::~Collection() {
    for (list<IWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        delete *itr;
    }
}
    
Vector<2,int> Collection::GetPosition() {
    return Vector<2,int>(x, y);
}

Vector<2,int> Collection::GetDimensions() {
    return Vector<2,int>(width, height);
}

void Collection::SetPosition(Vector<2,int> pos) {
    x = pos[0];
    y = pos[1];
    UpdateWidgets();
}

void Collection::SetDimensions(Vector<2,int> dim) {
    width = dim[0];
    height = dim[1];
}

void Collection::Accept(IWidgetRenderer& r) {
    r.Visit(this);
}

IWidget* Collection::WidgetAt(int x, int y) {
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height) {
        for (list<IWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
             itr++) {
            IWidget* w = (*itr)->WidgetAt(x,y);
            if (w) {
                return w;
            }
        }
        return this;
    }
    return NULL;
}

bool Collection::GetActive() {
    return active;
}

void Collection::SetActive(bool active) {
    this->active = active;
}

bool Collection::GetFocus() {
    return focus;
}

void Collection::SetFocus(bool focus) {
    this->focus = focus;
}

void Collection::AddWidget(IWidget* w) {
    widgets.push_back(w);
    UpdateWidgets();
}

void Collection::RemoveWidget(IWidget* w) {
    widgets.remove(w);
    UpdateWidgets();
}

IWidget* Collection::FocusAt(int x, int y) {
    if (active) {
        SetPosition(Vector<2,int>(x-dx, y-dy));
        return this;
    }
    focusWidget = NULL;
    for (list<IWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        IWidget* w = (*itr)->FocusAt(x,y);
        if (w) focusWidget = w;
    }
    if (focusWidget) {
        SetFocus(false);
        return focusWidget;
    }
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height) {
        dx = x - this->x;
        dy = y - this->y;
        SetFocus(true);
        return this;
    }
    SetFocus(false);
    return NULL;
}

IWidget* Collection::ActivateAt(int x, int y) {
    IWidget* w = NULL;
    if (mode == SIMPLE || focusWidget) {
        for (list<IWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
             itr++) {
            w = (*itr)->ActivateAt(x,y);
        }
    }
    return w;
}

IWidget* Collection::ActivateFocus() {
    if (!focusWidget) {
        if (GetFocus()) {
            SetActive(true);
            return this;
        }
        else return NULL;
    }
    if (mode == SIMPLE || mode == RADIO) {
       for (list<IWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
             itr++) {
           IWidget* w;
           if (w = (*itr)->ActivateFocus()) return w;
        }
    }
    if (mode == TOGGLE) {
       for (list<IWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
         itr++) {
            IWidget* w = (*itr); 
            if (w->GetFocus()) {
                w->SetActive(!w->GetActive());
                return w;
            }
       }
    }
    return NULL;
}

void Collection::Reset() {
    // in simple mode all widgets will be reset. (usually means
    // deactivated).

    // in radio mode active widgets will stay active (nothing is
    // reset). this gives a radio group-like behaviour.
   if (mode == SIMPLE) {
       for (list<IWidget*>::iterator itr = widgets.begin(); 
            itr != widgets.end();
            itr++) {
           (*itr)->Reset();
        }
    }
   SetActive(false);
}

list<IWidget*> Collection::GetWidgets() {
    return widgets;
}

void Collection::UpdateWidgets() {
    int i = 0;
    int acc_y = y;
    width = 0;
    for (list<IWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        IWidget* w = *itr;
        int y_space = 2;
        int w_y = y_space + acc_y;
        w->SetPosition(Vector<2,int>(x,w_y));
        i++;
        width = fmax(w->GetDimensions()[0], width);
        acc_y += w->GetDimensions()[1] + y_space;
    }
    height = acc_y-y;
}


} // NS Utils
} // NS OpenEngine
