// ITool - Selection Tool
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_SELECTION_TOOL_
#define _OE_UTILS_SELECTION_TOOL_

#include <Utils/ITool.h>
#include <Utils/PointingDevice.h>
#include <Scene/ISceneNodeVisitor.h>

namespace OpenEngine {
    namespace Renderers {
        class IRenderer;
    }
    namespace Display {
        class IViewingVolume;
    }
namespace Utils {

/**
 * Selection Tool
 * 
 * @class SelectionTool SelectionTool.h OpenGLSelection/Utils/SelectionTool.h
 */
class SelectionTool : public ITool {
private:
    int x, y, down_x, down_y;
public:
    SelectionTool();
    bool Handle(PointingDevice::MovedEventArg arg);
    bool Handle(PointingDevice::PressedEventArg arg);
    bool Handle(PointingDevice::ReleasedEventArg arg);
    void Render(Display::IViewingVolume& vv, Renderers::IRenderer& r);
    void RenderOrtho(Display::IViewingVolume& vv, Renderers::IRenderer& r);
};

} // NS Utils
} // NS OpenEngine
#endif 
