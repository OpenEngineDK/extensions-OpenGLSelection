// ITool - Widget tool (manipulate widgets)
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/WidgetTool.h>

#include <Widgets/IWidget.h>
#include <Widgets/IWidgetRenderer.h>
#include <Widgets/WidgetRenderer.h>
#include <Display/IViewingVolume.h>
#include <Renderers/IRenderer.h>

//#include <Logging/Logger.h>

namespace OpenEngine {
namespace Utils {

using namespace Display;
using namespace Renderers;
using namespace Widgets;
    
WidgetTool::WidgetTool(TextureLoader& texloader)
    : activew(false)
    , wr(new WidgetRenderer(texloader))
{
    
}

WidgetTool::~WidgetTool() {
    delete wr;
}

bool WidgetTool::Handle(PointingDevice::MovedEventArg arg) {
    for (list<IWidget*>::iterator itr = widgets.begin();
         itr != widgets.end();
         ++itr) {
        if ((*itr)->FocusAt(arg.state.x, arg.state.y)) {
            return true;
        }
    }
    return false;
}

bool WidgetTool::Handle(PointingDevice::PressedEventArg arg) {
    if (arg.btn == 0x1) {
        activew = false;
        for (list<IWidget*>::iterator itr = widgets.begin();
             itr != widgets.end();
             ++itr) {
            if ((*itr)->ActivateFocus()) {
                activew = true;
            }
        }
        if (activew) return true;
    }
    return false;
}

bool WidgetTool::Handle(PointingDevice::ReleasedEventArg arg) {
    if (activew) {
        for (list<IWidget*>::iterator itr = widgets.begin();
             itr != widgets.end();
             ++itr) {
            (*itr)->Reset();
        }
        activew = false;
        return true;
    }
    return false;
}

void WidgetTool::Render(IViewingVolume& vv, IRenderer& r) {
}
    
void WidgetTool::RenderOrtho(IViewingVolume& vv, Renderers::IRenderer& r) {
    wr->RenderWidgets();
}

void WidgetTool::AddWidget(IWidget* w) {
    w->SetPosition(Vector<2,int>(20,20));
    wr->AddWidget(w);
    widgets.push_back(w);
}

} // NS Widgets
} // NS OpenEngine

