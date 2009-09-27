// MouseSelection.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/MouseSelection.h>

#include <Display/IFrame.h>
#include <Display/Viewport.h>
#include <Display/OrthogonalViewingVolume.h>
#include <Scene/ISceneNode.h>
#include <Utils/GLSceneSelection.h>
#include <Utils/SelectionTool.h>
#include <Utils/TransformationTool.h>
#include <Utils/CameraTool.h>

#include <Logging/Logger.h>

#define dispatchEvent(e)                        \
  vtmap[activeViewport]->Handle(e);
 
namespace OpenEngine {
namespace Utils {

using namespace Display;
using namespace Devices;
using namespace Renderers;
using namespace Scene;
using namespace std;

MouseSelection::MouseSelection (IFrame& frame, 
                                IMouse& mouse, 
                                ISceneNode* root)
    : frame(frame) 
    , mouse(mouse)
    , scenesel(new GLSceneSelection(frame))
    , pd(new PointingDevice())
    , root (root)
    , activeViewport(NULL)
{
}

MouseSelection::~MouseSelection() {
    delete pd;
    delete scenesel;
}

void MouseSelection::Handle(MouseMovedEventArg arg) {
    // find the active viewport
    if (activeViewport == NULL || !IsViewportActive(activeViewport, arg.x, arg.y)) {
        // dont find new viewport if buttons are pressed
        if (pd->state.btns) return; 
        activeViewport = NULL;
        for (map<Viewport*,ITool*>::iterator itr = vtmap.begin();
             itr != vtmap.end();
             itr++) {
            if (IsViewportActive((*itr).first, arg.x, arg.y)) {
                activeViewport = (*itr).first;
                break;
            }
        }
        // for (list<Viewport*>::iterator itr = viewports.begin();
        //      itr != viewports.end();
        //      itr++) {
        //     if (IsViewportActive(*itr, arg.x, arg.y)) {
        //         activeViewport = *itr;
        //         break;
        //     }
        // }
    }
    if (!activeViewport) return;
    Vector<4,int> d = activeViewport->GetDimension();
    pd->state.x = arg.x-d[0];
    pd->state.y = (d[3]+d[1])-(frame.GetHeight()-arg.y); 
    PointingDevice::MovedEventArg e(arg.dx, arg.dy, *scenesel, root, *pd, *activeViewport);
    dispatchEvent(e);
}

void MouseSelection::Handle(MouseButtonEventArg arg) {
    if (!activeViewport) return;
    if (arg.type == EVENT_PRESS) {
        PointingDevice::PressedEventArg e(0, *scenesel, root, *pd, *activeViewport);
        if (arg.button & BUTTON_LEFT) {
            pd->state.btns |= 0x1;
            e.btn = 0x1;
        }
        if (arg.button & BUTTON_RIGHT) {
            pd->state.btns |= 0x2;
            e.btn = 0x2;
        }
        if (arg.button & BUTTON_MIDDLE) {
            pd->state.btns |= 0x4;
            e.btn = 0x4;
        }
        if (arg.button & BUTTON_WHEEL_UP) {
            e.btn = 0x8;
        }
        if (arg.button & BUTTON_WHEEL_DOWN) {
            e.btn = 0x10;
        }
        dispatchEvent(e);
    }
    if (arg.type == EVENT_RELEASE) {
        PointingDevice::ReleasedEventArg e(0, *scenesel, root, *pd, *activeViewport);
        if (arg.button & BUTTON_LEFT) {
            pd->state.btns ^= 0x1;
            e.btn = 0x1;
        }
        if (arg.button & BUTTON_RIGHT) {
            pd->state.btns ^= 0x2;
            e.btn = 0x2;
        }
        if (arg.button & BUTTON_MIDDLE) {
            pd->state.btns ^= 0x4;
            e.btn = 0x4;
        }
        dispatchEvent(e);
    }
}

void MouseSelection::Handle(KeyboardEventArg arg) {
    if (arg.type == EVENT_PRESS) {
        if (arg.sym == KEY_SPACE) 
            pd->state.shifts |= 0x1;
    }
    if (arg.type == EVENT_RELEASE) {
        if (arg.sym == KEY_SPACE)
            pd->state.shifts ^= 0x1;
    }
}

void MouseSelection::Handle(RenderingEventArg arg) {
    // for each viewport
    if (activeViewport != NULL) {
        // setup projection and view matrix 
        Viewport& viewport = *activeViewport;
        Vector<4,int> d = viewport.GetDimension();
        glViewport((GLsizei)d[0], (GLsizei)d[1], (GLsizei)d[2], (GLsizei)d[3]);
        arg.renderer.ApplyViewingVolume(*viewport.GetViewingVolume());

        //render each tool bottom-up.
        vtmap[activeViewport]->Render(*viewport.GetViewingVolume(), arg.renderer);
        OrthogonalViewingVolume ortho(1.0f, 2.0f, 
                                      /*left*/0.0, /*right*/d[2], 
                                      /*top*/0.0, /*bottom*/d[3]);
        arg.renderer.ApplyViewingVolume(ortho);
        //render each ortho-tool bottom-up.
        vtmap[activeViewport]->RenderOrtho(ortho, arg.renderer);
    }
}

bool MouseSelection::IsViewportActive(Viewport* viewport, int x, int y) {
    Vector<4,int> dim = viewport->GetDimension();
    int fheight = frame.GetHeight();
    if (x >= dim[0] && x <= dim[0] + dim[2] && fheight-y >= dim[1] && fheight-y <= dim[1]+dim[3]) {
        return true;
    }
    return false;
}

void MouseSelection::AddViewport(Viewport* viewport) {
    viewports.push_back(viewport);
}

// void MouseSelection::AddTool(ITool* tool) {
//     tools.push_back(tool);
// }

void MouseSelection::BindTool(Viewport* vp, ITool* t) {
    vtmap[vp] = t;
}

void MouseSelection::SetScene(ISceneNode* scene) {
    this->root = scene;
}

} // NS Utils
} // NS OpenEngine
