// ITool - Selection Tool
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/SelectionTool.h>
#include <Renderers/IRenderer.h>
#include <Scene/ISceneNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/SearchTool.h>
#include <Display/IViewingVolume.h>
#include <Display/IFrame.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>
#include <Meta/OpenGL.h>
#include <list>
#include <Geometry/Sphere.h>
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Utils {
    
using namespace Renderers;
using namespace Display;
using namespace Scene;
using namespace std;
using namespace Math;
using namespace Geometry;
    
SelectionTool::SelectionTool(SelectionSet<ISceneNode>& ss): down_x(-1), down_y(-1), ss(ss) {}

bool SelectionTool::Handle(PointingDevice::MovedEventArg arg) {
    if (down_x == -1) return true;
    x = arg.state.x;
    y = arg.state.y;
    return false;
}

bool SelectionTool::Handle(PointingDevice::PressedEventArg arg) {
    switch (arg.btn) {
    case 1:
        x = down_x = arg.state.x;
        y = down_y = arg.state.y;
        ss.Clear();
        break;
    };
    return false;
}

bool SelectionTool::Handle(PointingDevice::ReleasedEventArg arg) {
    switch (arg.btn) {
    case 1:
        // if we did not receive the down event...
        if (down_x == -1)
            return true;
        list<ISceneNode*> selection;
        if (down_x == arg.state.x && down_y == arg.state.y) {
            selection = arg.select.SelectPoint(down_x,
                                               down_y,
                                               arg.root, 
                                               arg.vp);
            if (!selection.empty())
                ss.Select(selection.front());
        }
        else {
            selection = arg.select.SelectRegion(down_x,
                                                down_y,
                                                arg.state.x,
                                                arg.state.y,
                                                arg.root, 
                                                arg.vp);
            ss.Select(selection);
        }
        down_x = -1;
        down_y = -1;
        break;
    };
    return false;
}
    
void SelectionTool::Render(IViewingVolume& vv, IRenderer& r) {
}

void SelectionTool::RenderOrtho(IViewingVolume& vv, Renderers::IRenderer& r) {
    if (down_x != -1) {
        glPushAttrib(GL_LIGHTING_BIT);
        glPushAttrib(GL_DEPTH_BUFFER_BIT);
        //draw selection region
        if (x == 0) x = 1;
        if (y == 0) y = 1;
        if (down_x == 0) down_x = 1;
        if (down_y == 0) down_y = 1;
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(2.0);
        glBegin(GL_QUADS);
        glColor4f(0.0, 0.0, 1.0, 0.3);
        glVertex3f(down_x, down_y, -1.0);
        glVertex3f(x, down_y, -1.0);
        glVertex3f(x, y, -1.0);
        glVertex3f(down_x, y, -1.0);
        glEnd();
        glDisable(GL_BLEND);
        glBegin(GL_LINE_STRIP);
        glColor4f(0.0, 0.0, 1.0, 1.0);
        glVertex3f(down_x, down_y, -1.0);
        glVertex3f(x, down_y, -1.0);
        glVertex3f(x, y, -1.0);
        glVertex3f(down_x, y, -1.0);
        glVertex3f(down_x, down_y, -1.0);
        glEnd();
        glPopAttrib();
        glPopAttrib();
    }
}

} // NS Utils
} // NS OpenEngine

