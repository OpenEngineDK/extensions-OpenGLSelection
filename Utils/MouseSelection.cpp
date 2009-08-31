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

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Utils {

using namespace Display;
using namespace Devices;
using namespace Renderers;
using namespace Scene;
using namespace std;

MouseSelection::MouseSelection (IFrame& frame, 
                                IMouse& mouse, 
                                SelectionSet<ISceneNode>& sset,
                                ISceneNode* root)
    : frame(frame) 
    , mouse(mouse)
    , scenesel(new GLSceneSelection(frame))
    , pd(new PointingDevice())
    , sset (sset)
    , root (root)
    , activeViewport(NULL)
{
    TransformationTool* tt = new TransformationTool();
    sset.ChangedEvent().Attach(*tt);
    tools.push_back(tt);
    tools.push_back(new SelectionTool());
}

MouseSelection::~MouseSelection() {
    delete pd;
    delete scenesel;
}

void MouseSelection::Handle(MouseMovedEventArg arg) {
    // find the active viewport
    if (activeViewport == NULL || !IsViewportActive(activeViewport, arg.x, arg.y)) {
        // dont find new viewport if mouse button is pressed
        if (arg.buttons & BUTTON_LEFT) return; 
        activeViewport = NULL;
        for (list<Viewport*>::iterator itr = viewports.begin();
             itr != viewports.end();
             itr++) {
            if (IsViewportActive(*itr, arg.x, arg.y)) {
                activeViewport = *itr;
                break;
            }
        }
    }
    if (!activeViewport) return;
    Vector<4,int> d = activeViewport->GetDimension();
    pd->state.x = arg.x-d[0];
    pd->state.y = (d[3]+d[1])-(frame.GetHeight()-arg.y); 
    PointingDevice::MovedEventArg e(arg.dx, arg.dy, *scenesel, root, sset, *pd, *activeViewport);
    for (std::list<ITool*>::iterator i = tools.begin();
         i != tools.end();
         i++) {
        if (!(*i)->Handle(e)) break;
    }
}

void MouseSelection::Handle(MouseButtonEventArg arg) {
    if (!activeViewport) return;
    if (arg.type == EVENT_PRESS) {
        PointingDevice::PressedEventArg e(0, *scenesel, root, sset, *pd, *activeViewport);
        if (arg.button & BUTTON_LEFT) {
            pd->state.btn1down = true;
            e.btn = 1;
        }
        if (arg.button & BUTTON_RIGHT) {
            pd->state.btn2down = true;
            e.btn = 2;
        }
        for (std::list<ITool*>::iterator i = tools.begin();
             i != tools.end();
             i++) {
            if (!(*i)->Handle(e)) break;
        }
    }
    if (arg.type == EVENT_RELEASE) {
        PointingDevice::ReleasedEventArg e(0, *scenesel, root, sset, *pd, *activeViewport);
        if (arg.button & BUTTON_LEFT) {
            pd->state.btn1down = false;
            e.btn = 1;
        }
        if (arg.button & BUTTON_RIGHT) {
            pd->state.btn2down = false;
            e.btn = 2;
        }
        for (std::list<ITool*>::iterator i = tools.begin();
             i != tools.end();
             i++) {
            if (!(*i)->Handle(e)) break;
        }
    }
}

void MouseSelection::Handle(RenderingEventArg arg) {
    // for each viewport
    // for (list<Viewport*>::iterator vp = viewports.begin(); 
    //      vp != viewports.end();
    //      vp++) {
    if (activeViewport != NULL) {
        // setup projection and view matrix 
        // Viewport& viewport = **vp;
        Viewport& viewport = *activeViewport;
        Vector<4,int> d = viewport.GetDimension();
        glViewport((GLsizei)d[0], (GLsizei)d[1], (GLsizei)d[2], (GLsizei)d[3]);
        arg.renderer.ApplyViewingVolume(*viewport.GetViewingVolume());

        //render each tool bottom-up.
        for (list<ITool*>::reverse_iterator i = tools.rbegin();
             i != tools.rend(); 
             i++) {
            (*i)->Render(*viewport.GetViewingVolume(), arg.renderer);
        }
        // OrthogonalViewingVolume ortho(1.0f, 2.0f, /*left*/d[0], /*right*/d[0]+d[2], 
        //                               /*top*/frame.GetHeight()-d[1]-d[3],
        //                               /*bottom*/frame.GetHeight()-d[1]);
        OrthogonalViewingVolume ortho(1.0f, 2.0f, 
                                      /*left*/0.0, /*right*/d[2], 
                                      /*top*/0.0, /*bottom*/d[3]);
        arg.renderer.ApplyViewingVolume(ortho);
        //render each ortho-tool bottom-up.
        for (list<ITool*>::reverse_iterator i = tools.rbegin();
             i != tools.rend(); 
             i++) {
            (*i)->RenderOrtho(ortho, arg.renderer);
        }        
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

void MouseSelection::SetSelectionSet(SelectionSet<ISceneNode>& sset) {
    this->sset = sset;
}

void MouseSelection::SetScene(ISceneNode* scene) {
    this->root = scene;
}

} // NS Utils
} // NS OpenEngine
