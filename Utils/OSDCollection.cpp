// On screen display - collection of widgets.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/OSDCollection.h>
#include <Utils/OSDIRenderer.h>

namespace OpenEngine {
namespace Utils {

using namespace Math;
using namespace std;

OSDCollection::OSDCollection(ResetMode mode)
    : mode(mode)
    , x(0) 
    , y(0) 
    , width(0) 
    , height(0)
      //, activeWidget(NULL)
    , focusWidget(NULL)
{}

OSDCollection::~OSDCollection() {}
    
Vector<2,int> OSDCollection::GetPosition() {
    return Vector<2,int>(x, y);
}

Vector<2,int> OSDCollection::GetDimensions() {
    return Vector<2,int>(width, height);
}

void OSDCollection::SetPosition(Vector<2,int> pos) {
    x = pos[0];
    y = pos[1];
    UpdateWidgets();
}

void OSDCollection::SetDimensions(Vector<2,int> dim) {
    width = dim[0];
    height = dim[1];
}

void OSDCollection::Accept(OSDIRenderer& r) {
    r.Render(*this);
}

OSDIWidget* OSDCollection::WidgetAt(int x, int y) {
    if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height) {
        for (list<OSDIWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
             itr++) {
            OSDIWidget* w = (*itr)->WidgetAt(x,y);
            if (w) {
                return w;
            }
        }
    }
    return NULL;
}

bool OSDCollection::GetActive() {
    for (list<OSDIWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        if ((*itr)->GetActive()) return true;
    }
    return false;
}

void OSDCollection::SetActive(bool active) {
}

bool OSDCollection::GetFocus() {
    return (focusWidget != NULL);
}

void OSDCollection::SetFocus(bool focus) {
}

void OSDCollection::AddWidget(OSDIWidget* w) {
    widgets.push_back(w);
    UpdateWidgets();
}

void OSDCollection::RemoveWidget(OSDIWidget* w) {
    widgets.remove(w);
    UpdateWidgets();
}

OSDIWidget* OSDCollection::FocusAt(int x, int y) {
    focusWidget = NULL;
    for (list<OSDIWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        OSDIWidget* w = (*itr)->FocusAt(x,y);
        if (w) focusWidget = w;
    }
    // if (focusWidget) focusWidget->SetFocus(false);
    // focusWidget = WidgetAt(x,y);
    // if (focusWidget) focusWidget->FocusAt(x, y);
    return focusWidget;
}

OSDIWidget* OSDCollection::ActivateAt(int x, int y) {
    OSDIWidget* w = NULL;
    if (mode == SIMPLE || focusWidget) {
        for (list<OSDIWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
             itr++) {
            w = (*itr)->ActivateAt(x,y);
        }
    }
    return w;
}

OSDIWidget* OSDCollection::ActivateFocus() {
    // deactivate all widgets
    if (mode == SIMPLE || focusWidget) {
        for (list<OSDIWidget*>::iterator itr = widgets.begin(); 
             itr != widgets.end();
             itr++) {
            (*itr)->SetActive(false);
        }
    }
    // activate the first encountered widget that has focus 
    for (list<OSDIWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        OSDIWidget* w = (*itr)->ActivateFocus();        
        if (w) return w;
    }
    return NULL;
}

void OSDCollection::Reset() {
    // in simple mode all widgets will be reset. (usually means
    // deactivated).

    // in radio mode active widgets will stay active (nothing is
    // reset). this gives a radio group-like behaviour.
   if (mode == SIMPLE) {
       for (list<OSDIWidget*>::iterator itr = widgets.begin(); 
            itr != widgets.end();
            itr++) {
           (*itr)->Reset();
        }
    }
}

list<OSDIWidget*> OSDCollection::GetWidgets() {
    return widgets;
}

void OSDCollection::UpdateWidgets() {
    int i = 0;
    int acc_y = y;
    width = 0;
    for (list<OSDIWidget*>::iterator itr = widgets.begin(); 
         itr != widgets.end();
         itr++) {
        OSDIWidget* w = *itr;
        int y_space = 10;
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
