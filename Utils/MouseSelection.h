// Mouse selection.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_MOUSE_SELECTION_
#define _OE_UTILS_MOUSE_SELECTION_

#include <Core/IListener.h>
#include <Devices/IMouse.h>
#include <Renderers/IRenderer.h>

// selection tool utils
#include <Utils/PointingDevice.h>
#include <Utils/SelectionSet.h>
#include <Utils/ISceneSelection.h>
#include <Utils/ITool.h>
// STL
#include <list>
#include <queue>

namespace OpenEngine {
    // forward declarations
    namespace Display {
        class Viewport;
        class IFrame;
    }
    namespace Scene {
        class ISceneNode;
    }
namespace Utils {
/**
 * Select objects using the mouse.
 *
 * @class MouseSelection MouseSelection.h OpenGLSelection/Selector/MouseSelection.h
 */
class MouseSelection : public Core::IListener<Devices::MouseMovedEventArg> 
                     , public Core::IListener<Devices::MouseButtonEventArg>
                     , public Core::IListener<Renderers::RenderingEventArg> {
private:
    Display::IFrame& frame;
    Devices::IMouse& mouse;
    ISceneSelection* scenesel;
    PointingDevice* pd;
    SelectionSet<Scene::ISceneNode>& sset;
    Scene::ISceneNode* root;
    Display::Viewport* activeViewport;
    std::list<Display::Viewport*> viewports;
    std::queue<PointingDevice::EventArg*> events;
    std::list<ITool*> tools;
    bool IsViewportActive(Display::Viewport* viewport, int x, int y);
public:
    MouseSelection(Display::IFrame& frame, 
                   Devices::IMouse& mouse, 
                   SelectionSet<Scene::ISceneNode>& sset, 
                   Scene::ISceneNode* root);
    virtual ~MouseSelection();

    void Handle(Devices::MouseMovedEventArg arg);
    void Handle(Devices::MouseButtonEventArg arg);
    void Handle(Renderers::RenderingEventArg arg);

    void AddViewport(Display::Viewport* viewport);
    void AddTool(ITool* tool);

    void SetSelectionSet(SelectionSet<Scene::ISceneNode>& sset);
    void SetScene(Scene::ISceneNode* scene);
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_MOUSE_SELECTION_
