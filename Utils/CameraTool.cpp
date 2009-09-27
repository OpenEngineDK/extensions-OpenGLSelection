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

namespace OpenEngine {
namespace Utils {
       
using namespace Display;
using namespace Renderers;
using namespace Math;

CameraTool::CameraTool(): m_x(-1), m_y(-1)  {}

bool CameraTool::Handle(PointingDevice::MovedEventArg arg) {
    if (m_x == -1) return true;
    if (!(arg.state.btns & 0x7))
        return true;
    IViewingVolume& vv = *arg.vp.GetViewingVolume();
    float step = 5.0f;
    int dx = arg.state.x - m_x;
    int dy = arg.state.y - m_y;
    if (arg.state.btns & 0x1) {
        Vector<3,float> cam_x(1.0f,0.0f,0.0f);
        Vector<3,float> cam_y(0.0f,-1.0f,0.0f);
        //vv.SetPosition(init_p + init_q.RotateVector(cam_x) * dx * step + init_q.RotateVector(cam_y)*dy*step);
        SetPosition(init_p + init_q.RotateVector(cam_x) * dx * step + 
                    init_q.RotateVector(cam_y) * dy * step, vv); 
        return false;
    }
    if (arg.state.btns & 0x2) {
        Vector<3,float> cam_x(1.0f,0.0f,0.0f);
        Vector<3,float> cam_z(0.0f,0.0f,-1.0f);
        // vv.SetPosition(init_p + 
        //                init_q.RotateVector(cam_x) * dx * step + 
        //                init_q.RotateVector(cam_z) * dy * step);
        SetPosition(init_p + 
                    init_q.RotateVector(cam_x) * dx * step + 
                    init_q.RotateVector(cam_z) * dy * step, vv);
        return false;
    }
    if (arg.state.btns & 0x4) { 
        Quaternion<float> cam_x(dy*0.01, init_q.RotateVector(Vector<3,float>(1.0f,0.0f,0.0f)));
        Quaternion<float> cam_y(dx*0.01, /*init_q.RotateVector(*/Vector<3,float>(0.0f,-1.0f,0.0f)/*)*/);
        // Vector<3,float> axis = init_q.RotateVector(Vector<3,float>(1.0f,0.0f,0.0f))*dy 
        //     + init_q.RotateVector(Vector<3,float>(0.0f,-1.0f,0.0f))*dx;
        Quaternion<float> cam_rot/*(0.1, axis);*/ = cam_y * cam_x;
        //vv.SetDirection( cam_rot.GetNormalize() * init_q );
        SetRotation(cam_rot.GetNormalize() * init_q, vv);
        return false;
    }
    return true;
}

bool CameraTool::Handle(PointingDevice::PressedEventArg arg) {
    if (!(arg.state.shifts & 0x1)) 
        return true;
    if (arg.btn & 0x7) {
        IViewingVolume& vv = *arg.vp.GetViewingVolume();
        init_q = vv.GetDirection();
        init_p = vv.GetPosition();
        m_x = arg.state.x;
        m_y = arg.state.y;
        return false;
    }
    if ((arg.btn == 0x8) || (arg.btn == 16)) {
        IViewingVolume& vv = *arg.vp.GetViewingVolume();
        Quaternion<float> q = vv.GetDirection();
        Vector<3,float> forward(0.0f,0.0f,1.0f);
        float step = 40.0f;
        if (arg.btn == 0x8) 
            step *= -1;
        //vv.SetPosition(vv.GetPosition() + q.RotateVector(forward)*step);
        SetPosition(vv.GetPosition() + q.RotateVector(forward)*step, vv);
        return false;
    }
    return true;
}

bool CameraTool::Handle(PointingDevice::ReleasedEventArg arg) {
    if (m_x != -1) {
        m_x = m_y = -1;
        return false;
    }
    return true;
}
    
void CameraTool::Render(IViewingVolume& vv, IRenderer& r) {
    if (timer_p.IsRunning()) {
        float t_p = (timer_p.GetElapsedTime().AsInt32() / (1000.0f) ) / max_time_p; 
        //logger.info << "t_p: " << t_p << logger.end;
        if (t_p > 1.0f) {
            t_p = 1.0f;
            timer_p.Stop();
        }
        vv.SetPosition(start_p + delta_p*t_p);
    }
    if (timer_q.IsRunning()) {
        float t_q = timer_q.GetElapsedTime().AsInt32() / (1000.0f) / max_time_q;
        //logger.info << "t_q: " << t_q << logger.end;
        if (t_q > 1.0f) {
            t_q = 1.0;
            timer_q.Stop();
        }
        vv.SetDirection(Quaternion<float>(start_q, end_q, t_q));
    }
}

void CameraTool::RenderOrtho(IViewingVolume& vv, Renderers::IRenderer& r) {
}

void CameraTool::SetPosition(Vector<3,float> p, IViewingVolume& vv) {
    start_p = vv.GetPosition();
    end_p = p;
    delta_p = end_p - start_p;
    float speed = 1.3; // miliseconds per unit distance
    max_time_p = speed * delta_p.GetLength();
    timer_p.Reset();
    timer_p.Start();
}

void CameraTool::SetRotation(Quaternion<float> q, IViewingVolume& vv) {
    start_q = vv.GetDirection();
    end_q = q;
    float speed = 200; // miliseconds per radian rotation.
    max_time_q = speed * fabs(q.GetReal());
    timer_q.Reset();
    timer_q.Start();
}

} // NS Utils
} // NS OpenEngine

