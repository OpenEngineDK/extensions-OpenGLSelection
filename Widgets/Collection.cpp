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

#include <Core/Exceptions.h>
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Widgets {

using namespace Math;
using namespace std;
using namespace Core;

Collection::Collection()
    : IWidget()
    , mode(SIMPLE)
    , fixed(false)
    , bg(true)
    , focusWidget(NULL)
    , spacing(5)
    , padding(Vector<4,int>(10))
{}

Collection::Collection(Mode mode)
    : IWidget()
    , mode(mode)
    , fixed(false)
    , bg(true)
    , focusWidget(NULL)
    , spacing(5)
    , padding(Vector<4,int>(10))
{}

Collection::~Collection() {
    for (list<IWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        delete *itr;
    }
}
    
void Collection::SetPosition(Vector<2,int> pos) {
    x = pos[0];
    y = pos[1];
    UpdateWidgets();
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
            if (w) return w;
        }
        return this;
    }
    return NULL;
}

void Collection::AddWidget(IWidget* w) {
    widgets.push_back(w);
    w->DimensionsChangedEvent().Attach(*this);
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
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height) {
        dx = x - this->x;
        dy = y - this->y;
        SetFocus(true);
    }
    else { 
        SetFocus(false);
        for (list<IWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
             itr++) {
            (*itr)->FocusAt(x,y);
        }
        // assume that no widget has focus if collection does not has focus.
        return NULL;
    }
    focusWidget = NULL;
    for (list<IWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        IWidget* w = (*itr)->FocusAt(x,y);
        if (w) focusWidget = w;
    }
    if (focusWidget) {
        return focusWidget;
    }
    return this;
}

IWidget* Collection::ActivateAt(int x, int y) {
    throw new Exception("ActiveAt method not implemented on Collection class.");
}

IWidget* Collection::ActivateFocus() {
    if (!GetFocus()) return NULL;
    IWidget* w = NULL;
    if (mode == SIMPLE) {
        for (list<IWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
             itr++) {
            (*itr)->Reset();
            IWidget* _w = (*itr)->ActivateFocus();
            if (_w) w = _w;
        }
        if (w) {
            return w;
        }
    }
    if (mode == RADIO) {
        bool f = false;
        for (list<IWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
             itr++) {
            if ((*itr)->GetFocus()) {
                f = true;
                break;
            }
        }
        if (f) {
            for (list<IWidget*>::iterator itr = widgets.begin(); 
                 itr != widgets.end();
                 itr++) {
                (*itr)->Reset();
                IWidget* _w = (*itr)->ActivateFocus();
                if (_w) w = _w;
            }
            if (w) {
                return w;
            }
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
    if (GetFocus() && !fixed) {
        SetActive(true);
        return this;
    }
    else return NULL;
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
    int acc_y = y - spacing + padding[1];
    int _width = 0;
    for (list<IWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        IWidget* w = *itr;
        int w_y = spacing + acc_y;
        w->SetPosition(Vector<2,int>(x + padding[0], w_y));
        i++;
        _width = fmax(w->GetDimensions()[0], _width);
        acc_y += w->GetDimensions()[1] + spacing;
    }
    this->SetDimensions(Vector<2,int>(_width + padding[0] + padding[2],
                                acc_y - y + padding[3]));
}

void Collection::SetSpacing(int spacing) {
    this->spacing = spacing;
    UpdateWidgets();
}

int Collection::GetSpacing() {
    return spacing;
}

void Collection::SetPadding(Vector<4,int> padding) {
    this->padding = padding;
    UpdateWidgets();
}

Vector<4,int> Collection::GetPadding() {
    return padding;
}

void Collection::SetFixed(bool fixed) {
    this->fixed = fixed;
}

bool Collection::GetFixed() {
    return fixed;
}

void Collection::SetBackground(bool bg) {
    this->bg = bg;
}

bool Collection::GetBackground() {
    return bg;
}


void Collection::Handle(DimensionsChangedEventArg e) {
    UpdateWidgets();
}

} // NS Utils
} // NS OpenEngine
