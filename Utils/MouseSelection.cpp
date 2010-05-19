// MouseSelection.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/MouseSelection.h>

#include <Display/Viewport.h>
#include <Display/OrthogonalViewingVolume.h>
#include <Display/IRenderCanvas.h>
#include <Scene/ISceneNode.h>
#include <Meta/OpenGL.h>
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Utils {

using namespace Display;
using namespace Devices;
using namespace Renderers;
using namespace Scene;
using namespace std;

MouseSelection::MouseSelection (IMouse& mouse,
                                IKeyboard& keyboard)
    : mouse(mouse)
    , pd(new PointingDevice())
    , activeCanvas(NULL)
{
    mouse.MouseMovedEvent().Attach(*this);
    mouse.MouseButtonEvent().Attach(*this);
    keyboard.KeyEvent().Attach(*this);
}

MouseSelection::~MouseSelection() {
    delete pd;
}

void MouseSelection::Handle(MouseMovedEventArg arg) {
    // find the active viewport
    if (activeCanvas == NULL || !IsCanvasActive(activeCanvas, arg.x, arg.y)) {
        // dont find new canvas if buttons are pressed
        if (pd->state.btns) return; 
        activeCanvas = NULL;
        for (map<IRenderCanvas*,ITool*>::iterator itr = vtmap.begin();
             itr != vtmap.end();
             itr++) {
            if (IsCanvasActive((*itr).first, arg.x, arg.y)) {
                activeCanvas = (*itr).first;
                break;
            }
        }
    }
    if (!activeCanvas) return;
    Vector<2,int> pos = activeCanvas->GetPosition();
    pd->state.x = arg.x - pos[0];
    pd->state.y = arg.y - pos[1]; 
    PointingDevice::MovedEventArg e(arg.dx, arg.dy, *pd, *activeCanvas);
    vtmap[activeCanvas]->Handle(e);
}

void MouseSelection::Handle(MouseButtonEventArg arg) {
    if (!activeCanvas) return;
    if (arg.type == EVENT_PRESS) {
        PointingDevice::PressedEventArg e(0, *pd, *activeCanvas);
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
        vtmap[activeCanvas]->Handle(e);
    }
    if (arg.type == EVENT_RELEASE) {
        PointingDevice::ReleasedEventArg e(0, *pd, *activeCanvas);
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
        vtmap[activeCanvas]->Handle(e);
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
    if (activeCanvas != NULL) {
        glPushAttrib(GL_LIGHTING_BIT);
        glPushAttrib(GL_ENABLE_BIT);
        glPushAttrib(GL_DEPTH_BUFFER_BIT);
        glPushAttrib(GL_COLOR_BUFFER_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        if (activeCanvas) {
            IRenderCanvas* rc = &arg.canvas;//*activeCanvas;
            // logger.info << "acc: " << activeCanvas << logger.end;;
            // logger.info << "argc: " << rc << logger.end;;
            if ( rc == activeCanvas) {
                logger.info << "hit canvas: " << activeCanvas << logger.end;;
            }            
        }


        // setup projection and view matrix 
        // for (map<IRenderCanvas*, ITool*>::iterator itr = vtmap.begin();
        //      itr != vtmap.end();
        //      ++itr) {
        //     IRenderCanvas* canvas = (*itr).first;            
            //render each tool bottom-up.
            // vtmap[arg.canvas]->Render(canvas->GetViewingVolume(), arg.renderer);
            // OrthogonalViewingVolume ortho(1.0f, 2.0f, 
            //                               0.0, arg.canvas->GetWidth(), 
            //                               0.0, arg.canvas->GetHeight());
            // arg.renderer.ApplyViewingVolume(ortho);
            //render each ortho-tool bottom-up.
            // vtmap[canvas]->RenderOrtho(ortho, arg.renderer);
        // }   
        glPopAttrib();
        glPopAttrib();
        glPopAttrib();
        glPopAttrib();
    }
}

bool MouseSelection::IsCanvasActive(IRenderCanvas* canvas, int x, int y) {
    Vector<2,int> pos = canvas->GetPosition();
    Vector<4,int> dim(pos[0], pos[1], canvas->GetWidth(), canvas->GetHeight());
    if (x >= dim[0] && x <= dim[0] + dim[2] && y >= dim[1] && y <= dim[1]+dim[3]) {
        return true;
    }
    return false;
}

void MouseSelection::BindTool(IRenderCanvas* canvas, ITool* tool) {
    vtmap[canvas] = tool;
}

} // NS Utils
} // NS OpenEngine
