// MouseSelector.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/MouseSelector.h>

#include <Display/IFrame.h>
#include <Display/Camera.h>
#include <Devices/IMouse.h>
#include <Display/IViewingVolume.h>
#include <Display/ViewingVolume.h>
#include <Display/Viewport.h>
#include <Geometry/Line.h>
#include <Math/Quaternion.h>
#include <Math/Math.h>
#include <Display/OrthogonalViewingVolume.h>

#include <Scene/ISceneNode.h>

#include <Geometry/Tests.h>
#include <Geometry/Ray.h>
#include <Geometry/Box.h>

#include <Scene/SearchTool.h>
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Utils {

using Display::IViewingVolume;
using Display::ViewingVolume;
using Display::OrthogonalViewingVolume;
using Display::Viewport;
using Display::Camera;
using Math::Quaternion;
using Geometry::Line;
using Geometry::Tests;
using Geometry::Ray;
using Geometry::Box;
using Scene::SearchTool;

MouseSelector::MouseSelector (IFrame& frame, 
                              IMouse& mouse, 
                              SelectionSet<ISceneNode>& sset,
                              ISceneNode* root)
    : frame(frame) 
    , mouse(mouse)
    , sset (sset)
    , root (root)
    , activeViewport(NULL)
    , down_x(-1)
    , down_y(-1)
{
}

MouseSelector::~MouseSelector() {
}

void MouseSelector::Handle(MouseMovedEventArg arg) {
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

    int dx = arg.dx;//(arg.x - mx);
    int dy = arg.dy;//(arg.y - my);
        MoveCamera(0.1*dx, 0.1*dy);

    if (arg.buttons & BUTTON_RIGHT ) {
        logger.info << "move" << logger.end;
        return;
    }

}

void MouseSelector::Handle(MouseButtonEventArg arg) {
    if (!activeViewport) return;

    if (arg.type == EVENT_PRESS) {
        if (arg.button & BUTTON_WHEEL_UP)   { ZoomIn();  } 
        if (arg.button & BUTTON_WHEEL_DOWN) { ZoomOut(); } 
        if (arg.button & BUTTON_LEFT) {
            down_x = arg.state.x;
            down_y = arg.state.y;
        }
    }
    if (arg.type == EVENT_RELEASE) {
        if (arg.button & BUTTON_LEFT) {
            sset.Clear();
            if (down_x == arg.state.x && down_y == arg.state.y) {
                list<ISceneNode*> sel = scenesel.SelectPoint(arg.state.x,
                                                             frame.GetHeight()-arg.state.y,
                                                             root, *activeViewport);
                if (!sel.empty()) {
                    // select only the topmost element.
                    sset.Select(sel.front());
                }
            }
            else {
                
                list<ISceneNode*> sel = scenesel.SelectRegion(down_x,
                                                              frame.GetHeight()-down_y,
                                                              arg.state.x,
                                                              frame.GetHeight()-arg.state.y,
                                                              root, *activeViewport);
                for (list<ISceneNode*>::iterator itr = sel.begin();
                     itr != sel.end();
                     itr++) {
                    sset.AddToSelection(*itr);
                    //logger.info << "hit" << logger.end;
                }
            }
        }
        down_x = down_y = -1;
    }
}

void MouseSelector::Handle(RenderingEventArg arg) {
    IRenderer& r = arg.renderer;
    
    // reusable locals
    float size;
    Vector<3,float> colr;
    Line l(colr, colr);

    // for each viewport ...
    for (list<Viewport*>::iterator vpitr = viewports.begin();
         vpitr != viewports.end();
         vpitr++) {
        // setup projection matrix 
        Viewport& viewport = **vpitr;
        Vector<4,int> d = viewport.GetDimension();
        glViewport((GLsizei)d[0], (GLsizei)d[1], (GLsizei)d[2], (GLsizei)d[3]);
        arg.renderer.ApplyViewingVolume(*viewport.GetViewingVolume());
        
        // outline selected objects
        // @todo: room for optimizations such as: 
        //        - store bounding boxes.
        //        - only calculate transformations once.
        colr = Vector<3,float> (0.0,0.0,1.0);
        set<ISceneNode*> sel = sset.GetSelection();
        for (set<ISceneNode*>::iterator itr = sel.begin();
             itr != sel.end(); 
             itr++) {
            
            Box b(**itr);
            Vector<3,float> p;
            Quaternion<float> q;
            Vector<3,float> s;
            
            SearchTool st;
            TransformationNode* t;
            t = st.AncestorTransformationNode(*itr);
            if (t) {
                t->GetAccumulatedTransformations(&p, &q, &s);
            }
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(p[0], p[1], p[2]);
            p = q.GetImaginary();
            glRotatef(q.GetReal(), p[0], p[1], p[2]);
            glScalef(s[0], s[1], s[2]);
            size = 1;
            
            for (int i = 0; i < 8; i++) {
                for (int j = i; j < 8; j++) {
                    if (i == j) continue;
                    l = Line(b.GetCorner(i), b.GetCorner(j));
                    r.DrawLine(l, colr, size);
                }
            }
            glPopMatrix();
        }
    }
    
    if (down_x == -1 || !activeViewport) return;//  || moving) return;
    //draw selection region
    //@todo: do not use opengl directly. optimize coordinate calculations?
    Vector<4,int> d = activeViewport->GetDimension();
    glViewport((GLsizei)d[0], (GLsizei)d[1], (GLsizei)d[2], (GLsizei)d[3]);
    
    OrthogonalViewingVolume ortho(1.0f, 2.0f, d[0]/*left*/, d[0]+d[2]/*right*/, 
                                  frame.GetHeight()-d[1]-d[3]/*top*/,
                                  frame.GetHeight()-d[1]/*bottom*/);
    
    r.ApplyViewingVolume(ortho);
    
    int x = mouse.GetState().x;
    int y = mouse.GetState().y;
    
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_QUADS);
    glColor4f(0.0,0.0,1.0,0.3);
    glVertex3f(down_x, down_y, -1.0);
    glVertex3f(x, down_y, -1.0);
    glVertex3f(x, y, -1.0);
    glVertex3f(down_x, y, -1.0);
    glEnd();
    glPopAttrib();
    
    colr = Vector<3,float>(0.0,0.0,1.0);
    size = 1;
    l = Line(Vector<3,float> (down_x, down_y, -1.0), 
           Vector<3,float> (x, down_y, -1.0));
    r.DrawLine(l, colr, size);

    l = Line(Vector<3,float> (down_x, down_y, -1.0), 
             Vector<3,float> (down_x, y, -1.0));
    r.DrawLine(l, colr, size);

    l = Line(Vector<3,float> (x, y, -1.0), 
             Vector<3,float> (down_x, y, -1.0));
    r.DrawLine(l, colr, size);

    l = Line(Vector<3,float> (x, y, -1.0), 
             Vector<3,float> (x, down_y, -1.0));
    r.DrawLine(l, colr, size);

}
    
// void MouseSelector::InitMoveSelection(float x, float y, Vector<3,float> startPos) {
//     // construct plane and calc initial ray intersection
//     plane = Plane(activeViewport->GetViewingVolume()->
//                   GetDirection().RotateVector(Vector<3,float>(0.0,0.0,1.0)),
//                   startPos);
//     Tests::Intersects(ts.GetProjectedRay(x, y, *activeViewport), plane, &startp);       
// }

// void MouseSelector::MoveSelection(float x, float y) {
//     // move selected object according to plane and ray intersection
//     Vector<3,float> tmppos;
//     Tests::Intersects(plane, ts.GetProjectedRay(x, y, *activeViewport), &tmppos);
//     Vector<3,float> dpos = tmppos-startp;
//     startp = tmppos;
//     MoveSelection (dpos);
// }

bool MouseSelector::IsViewportActive(Viewport* viewport, int x, int y) {
    Vector<4,int> dim = viewport->GetDimension();
    int fheight = frame.GetHeight();
    if (x >= dim[0] && x <= dim[0] + dim[2] && fheight-y >= dim[1] && fheight-y <= dim[1]+dim[3]) {
        return true;
    }
    return false;
}

void MouseSelector::AddViewport(Viewport* viewport) {
    viewports.push_back(viewport);
}

// Camera updating routines.
// @todo: SHOULD NOT BE PART OF THIS CLASS

void MouseSelector::ZoomIn() {
    Camera cam(*activeViewport->GetViewingVolume());
    cam.SetPosition(cam.GetPosition() + 
                    cam.GetDirection().RotateVector(Vector<3,float>(0.0,0.0,-1.0))*20);
}

void MouseSelector::ZoomOut() {
    Camera cam(*activeViewport->GetViewingVolume());
    cam.SetPosition(cam.GetPosition() - 
                    cam.GetDirection().RotateVector(Vector<3,float>(0.0,0.0,-1.0))*20);
}
    
// void MouseSelector::Rotate(float dx, float dy) {
//     Camera cam(*activeViewport->GetViewingVolume());
    
//     Vector<3,float>   target = cam.GetPosition();
//     Quaternion<float> rot = cam.GetDirection().GetNormalize();
//     target += rot.RotateVector(Vector<3,float>(0.0,0.0,-1.0))*rotationDepth;
//     Vector<3,float> dir = target - cam.GetPosition();
    
//     Quaternion<float> q1(0.05*dx, rot.RotateVector(Vector<3,float>(0.0,1.0,0.0)));
//     q1.Normalize();

//     Quaternion<float> q2(0.05*dy, rot.RotateVector(Vector<3,float>(1.0,0.0,0.0)));
//     q2.Normalize();
    
//     cam.SetPosition(target - q1.RotateVector(q2.RotateVector(dir)));
//     cam.LookAt(target);
// }

void MouseSelector::MoveCamera(float dx, float dy) {
    Camera cam(*activeViewport->GetViewingVolume());

    Vector<3,float>   pos = cam.GetPosition();
    Quaternion<float> rot = cam.GetDirection().GetNormalize();
    pos += rot.RotateVector(Vector<3,float>(-1.0,0.0,0.0))*dx;
    pos += rot.RotateVector(Vector<3,float>(0.0,1.0,0.0))*dy;
    
    cam.SetPosition(pos);
}


// selection manipulation
void MouseSelector::SetSelectionSet(SelectionSet<ISceneNode*>& sset) {
    sset = sset;
}

// Vector<3,float> MouseSelector::GetSelectionPos() {
//     if (selection.empty())
//         return Vector<3,float>(); // Exception maybe?

//     Vector<3,float> acc;
//     for (list<TransformationNode*>::iterator itr = selection.begin();
//          itr != selection.end();
//          itr++) 
//         acc += (*itr)->GetPosition();
//     acc *= 1/selection.size();
//     return selection.front()->GetPosition();
// }

// void MouseSelector::MoveSelection(Vector<3,float> dpos) {
//     for (list<TransformationNode*>::iterator itr = selection.begin();
//          itr != selection.end();
//          itr ++)
//         (*itr)->SetPosition ((*itr)->GetPosition() + dpos);
// }

void MouseSelector::SetScene(ISceneNode* scene) {
    root = scene;
}

} // NS Utils
} // NS OpenEngine
