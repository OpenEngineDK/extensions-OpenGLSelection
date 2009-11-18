// ITool - Widget tool (manipulate widgets)
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_WIDGET_TOOL_
#define _OE_UTILS_WIDGET_TOOL_

#include <Utils/ITool.h>
#include <Utils/PointingDevice.h>
#include <Core/IListener.h>
#include <Math/Vector.h>

#include <list>

namespace OpenEngine {
    namespace Renderers {
        class TextureLoader;
    }
    namespace Widgets {
        class IWidget;
        class WidgetRenderer;
    }
namespace Utils {

using Renderers::TextureLoader;
using Widgets::IWidget;
using Widgets::WidgetRenderer;
using std::list;

/**
 * Widget Tool
 * Used to manipulate Widgets.
 * @class WidgetTool WidgetTool.h OpenGLSelection/Utils/WidgetTool.h
 */
class WidgetTool : public ITool {
private:
    list<IWidget*> widgets;
    bool activew;
    WidgetRenderer* wr;
public:
    WidgetTool(TextureLoader& texloader);
    ~WidgetTool();
    bool Handle(PointingDevice::MovedEventArg arg);
    bool Handle(PointingDevice::PressedEventArg arg);
    bool Handle(PointingDevice::ReleasedEventArg arg);
    void Handle(SelectionSet<Scene::ISceneNode>::ChangedEventArg arg);
    void Render(Display::IViewingVolume& vv, Renderers::IRenderer& r);
    void RenderOrtho(Display::IViewingVolume& vv, Renderers::IRenderer& r);
    void AddWidget(IWidget* w);
};

} // NS Utils
} // NS OpenEngine
#endif 
