// ITool - Camera Handling Tool
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/CameraTool.h>
#include <Logging/Logger.h>

#include <Display/IViewingVolume.h>
#include <Display/Viewport.h>
#include <Renderers/IRenderer.h>
#include <Math/Quaternion.h>

namespace OpenEngine {
namespace Utils {
       
using namespace Display;
using namespace Renderers;
using namespace Math;

CameraTool::CameraTool()  {}

bool CameraTool::Handle(PointingDevice::MovedEventArg arg) {
    if (arg.state.btns & 0x1) {
        IViewingVolume& vv = *arg.vp.GetViewingVolume();
        Quaternion<float> q = vv.GetDirection();
        Vector<3,float> cam_x(1.0f,0.0f,0.0f);
        Vector<3,float> cam_y(0.0f,-1.0f,0.0f);
        float step = 5.0f;
        vv.SetPosition(vv.GetPosition() + q.RotateVector(cam_x)*arg.dx*step + q.RotateVector(cam_y)*arg.dy*step);
        return false;
    }
    return true;
}

bool CameraTool::Handle(PointingDevice::PressedEventArg arg) {
    if ((arg.btn == 0x8) || (arg.btn == 0xF)) {
        IViewingVolume& vv = *arg.vp.GetViewingVolume();
        Quaternion<float> q = vv.GetDirection();
        Vector<3,float> unit(0.0f,0.0f,1.0f);
        float step = 10.0f;
        if (arg.btn == 0x8) 
            step *= -1;
        vv.SetPosition(vv.GetPosition() + q.RotateVector(unit)*step);
        return false;
    }
    return true;
}

bool CameraTool::Handle(PointingDevice::ReleasedEventArg arg) {
    return true;
}
    
void CameraTool::Render(IViewingVolume& vv, IRenderer& r) {

}

void CameraTool::RenderOrtho(IViewingVolume& vv, Renderers::IRenderer& r) {
}

} // NS Utils
} // NS OpenEngine

