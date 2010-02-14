// ITool - Camera Handling Tool
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/CameraTool.h>

#include <Display/IViewingVolume.h>
#include <Display/Viewport.h>
#include <Renderers/IRenderer.h>

namespace OpenEngine {
namespace Utils {
       
using namespace Display;
using namespace Renderers;
using namespace Math;

CameraTool::CameraTool(): m_x(-1), m_y(-1), step(1.5), meta(true)  {}

CameraTool::CameraTool(bool meta): m_x(-1), m_y(-1), step(1.5), meta(meta)  {}

bool CameraTool::Handle(PointingDevice::MovedEventArg arg) {
    if (m_x == -1) return false;
    if (!(arg.state.btns & 0x7))
        return false;
    IViewingVolume& vv = *arg.vp.GetViewingVolume();
    int dx = arg.state.x - m_x;
    int dy = arg.state.y - m_y;
    if (arg.state.btns & 0x1) {
        Vector<3,float> cam_x(1.0f,0.0f,0.0f);
        Vector<3,float> cam_y(0.0f,-1.0f,0.0f);
        vv.SetPosition(init_p + init_q.RotateVector(cam_x) * dx * step 
                       + init_q.RotateVector(cam_y) * dy * step);
        return true;
    }
    if (arg.state.btns & 0x4) {
        Vector<3,float> cam_x(1.0f,0.0f,0.0f);
        Vector<3,float> cam_z(0.0f,0.0f,-1.0f);
        vv.SetPosition(init_p + init_q.RotateVector(cam_x) * dx * step 
                       + init_q.RotateVector(cam_z) * dy * step);
        return true;
    }
    if (arg.state.btns & 0x2) { 
        Quaternion<float> cam_x(dy*0.01, init_q.RotateVector(Vector<3,float>(1.0f,0.0f,0.0f)));
        Quaternion<float> cam_y(dx*0.01, Vector<3,float>(0.0f,-1.0f,0.0f));
        Quaternion<float> cam_rot = cam_y * cam_x;
        vv.SetDirection( cam_rot.GetNormalize() * init_q );
        return true;
    }
    return false;
}

bool CameraTool::Handle(PointingDevice::PressedEventArg arg) {
    if (!(arg.state.shifts & 0x1) && meta) 
        return false;
    if (arg.btn & 0x7) {
        IViewingVolume& vv = *arg.vp.GetViewingVolume();
        init_q = vv.GetDirection();
        init_p = vv.GetPosition();
        m_x = arg.state.x;
        m_y = arg.state.y;
        return true;
    }
    if ((arg.btn == 0x8) || (arg.btn == 16)) {
        IViewingVolume& vv = *arg.vp.GetViewingVolume();
        Quaternion<float> q = vv.GetDirection();
        Vector<3,float> forward(0.0f,0.0f,1.0f);
        float step = this->step * 10;
        if (arg.btn == 0x8) 
            step *= -1;
        vv.SetPosition(vv.GetPosition() + q.RotateVector(forward)*step);
        return true;
    }
    return false;
}

bool CameraTool::Handle(PointingDevice::ReleasedEventArg arg) {
    if (m_x != -1) {
        m_x = m_y = -1;
        return true;
    }
    return false;
}
     
void CameraTool::Render(IViewingVolume& vv, IRenderer& r) {
}

void CameraTool::RenderOrtho(IViewingVolume& vv, Renderers::IRenderer& r) {
}

} // NS Utils
} // NS OpenEngine

