// Selection Tool Interface
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_TOOL_CHAIN_
#define _OE_UTILS_TOOL_CHAIN_

#include <Utils/PointingDevice.h>
#include <Utils/ITool.h>

// STL
#include <list>

namespace OpenEngine {
    namespace Renderers {
        class IRenderer;
    }
    namespace Display {
        class IViewingVolume;
    }
namespace Utils {

/**
 * Compound tool containing several single tools. 
 * 
 * @class ToolChain ToolChain.h OpenGLSelection/Utils/ToolChain.h
 */
class ToolChain: public ITool {
private:
    std::list<ITool*> tools;
public:
    ToolChain();
    virtual ~ToolChain();

    bool Handle(PointingDevice::MovedEventArg arg);
    bool Handle(PointingDevice::PressedEventArg arg);
    bool Handle(PointingDevice::ReleasedEventArg arg);
    void Render(Display::IViewingVolume& vv, Renderers::IRenderer& r);
    void RenderOrtho(Display::IViewingVolume& vv, Renderers::IRenderer& r);

    void PushBackTool(ITool* tool);
};

} // NS Utils
} // NS OpenEngine
#endif 
