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
#include <Utils/SelectionSet.h>
#include <Renderers/IRenderer.h>
#include <Core/IListener.h>

namespace OpenEngine {
    namespace Renderers {
        //class IRenderer;
    }
    namespace Display {
        class IViewingVolume;
    }
namespace Utils {

    class ISceneSelection;
    
/**
 * Selection Tool
 * 
 * @class SelectionTool SelectionTool.h OpenGLSelection/Utils/SelectionTool.h
 */
class SelectionTool : public ITool, public Core::IListener<Renderers::RenderingEventArg> {
private:
    int x, y, down_x, down_y;
    SelectionSet<Scene::ISceneNode>& ss;
    ISceneSelection* sceneselection;
public:
    SelectionTool(SelectionSet<Scene::ISceneNode>& ss);
    bool Handle(PointingDevice::MovedEventArg arg);
    bool Handle(PointingDevice::PressedEventArg arg);
    bool Handle(PointingDevice::ReleasedEventArg arg);
    void Render(Display::IViewingVolume& vv, Renderers::IRenderer& r);
    void RenderOrtho(Display::IViewingVolume& vv, Renderers::IRenderer& r);
    void Handle(Renderers::RenderingEventArg arg);
};

} // NS Utils
} // NS OpenEngine
#endif 
