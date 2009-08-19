// Selection Tool Interface
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_INTERFACE_TOOL_
#define _OE_UTILS_INTERFACE_TOOL_

#include <Utils/PointingDevice.h>

namespace OpenEngine {
    namespace Renderers {
        class IRenderer;
    }
    namespace Display {
        class IViewingVolume;
    }
namespace Utils {

/**
 * Interface for a selection tool. 
 * 
 * @class ITool ITool.h OpenGLSelection/Utils/ITool.h
 */
class ITool {
public:
    virtual ~ITool() {};
    virtual bool Handle(PointingDevice::MovedEventArg arg) = 0;
    virtual bool Handle(PointingDevice::PressedEventArg arg) = 0;
    virtual bool Handle(PointingDevice::ReleasedEventArg arg) = 0;
    virtual void Render(Display::IViewingVolume& vv, Renderers::IRenderer& r) = 0;
    virtual void RenderOrtho(Display::IViewingVolume& vv, Renderers::IRenderer& r) = 0;
};

} // NS Utils
} // NS OpenEngine
#endif 
