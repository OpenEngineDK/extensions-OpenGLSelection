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
    float step;
    Math::Vector<3,float> init_p;
    Math::Quaternion<float> init_q;
    bool meta;
public:
    CameraTool();
    CameraTool(bool meta);
    bool Handle(PointingDevice::MovedEventArg arg);
    bool Handle(PointingDevice::PressedEventArg arg);
    bool Handle(PointingDevice::ReleasedEventArg arg);
    void Render(Display::IViewingVolume& vv, Renderers::IRenderer& r);
    void RenderOrtho(Display::IViewingVolume& vv, Renderers::IRenderer& r);
};

} // NS Utils
} // NS OpenEngine
#endif 
