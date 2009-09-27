// ITool - Camera Handling Tool
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_CAMERA_TOOL_
#define _OE_UTILS_CAMERA_TOOL_

#include <Utils/ITool.h>
#include <Utils/PointingDevice.h>
#include <Utils/Timer.h>
#include <Math/Quaternion.h>

namespace OpenEngine {
    namespace Renderers {
        class IRenderer;
    }
    namespace Display {
        class IViewingVolume;
    }
namespace Utils {
/**
 * Camera Tool
 * 
 * @class CameraTool CameraTool.h OpenGLSelection/Utils/CameraTool.h
 */
class CameraTool : public ITool {
private:
    int m_x, m_y;  //save mouse coordinates on button down.
    //interpolation stuff
    Timer timer_p, timer_q;
    float max_time_p, max_time_q;
    Math::Vector<3,float> init_p, start_p, delta_p, end_p;
    Math::Quaternion<float> init_q, start_q, end_q;
    void SetPosition(Math::Vector<3,float> p, Display::IViewingVolume& vv);
    void SetRotation(Math::Quaternion<float> q, Display::IViewingVolume& vv);
public:
    CameraTool();
    bool Handle(PointingDevice::MovedEventArg arg);
    bool Handle(PointingDevice::PressedEventArg arg);
    bool Handle(PointingDevice::ReleasedEventArg arg);
    void Render(Display::IViewingVolume& vv, Renderers::IRenderer& r);
    void RenderOrtho(Display::IViewingVolume& vv, Renderers::IRenderer& r);
};

} // NS Utils
} // NS OpenEngine
#endif 
