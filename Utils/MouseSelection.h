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
#include <Devices/IKeyboard.h>
#include <Renderers/IRenderer.h>

// selection tool utils
#include <Utils/PointingDevice.h>
#include <Utils/ITool.h>
// STL
#include <map>

namespace OpenEngine {
    // forward declarations
    namespace Display {
        class IRenderCanvas;
    }
namespace Utils {
    using Display::IRenderCanvas;
    using Devices::IMouse;
    using Devices::IKeyboard;
    using Devices::MouseMovedEventArg;
    using Devices::MouseButtonEventArg;
    using Devices::KeyboardEventArg;
    using Renderers::RenderingEventArg;

    using std::map;
/**
 * Select objects using the mouse.
 *
 * @class MouseSelection MouseSelection.h OpenGLSelection/Selector/MouseSelection.h
 */
class MouseSelection : public Core::IListener<Devices::MouseMovedEventArg> 
                     , public Core::IListener<Devices::MouseButtonEventArg>
                     , public Core::IListener<Devices::KeyboardEventArg>
                     , public Core::IListener<Renderers::RenderingEventArg> {
private:
    IMouse& mouse;
    PointingDevice* pd;
    IRenderCanvas* activeCanvas; // assuming disjoint canvases
    map<IRenderCanvas*, ITool*> vtmap;
    inline bool IsCanvasActive(IRenderCanvas* canvas, int x, int y);
public:
    MouseSelection(IMouse& mouse,
                   IKeyboard& keyboard);
    virtual ~MouseSelection();

    void Handle(MouseMovedEventArg arg);
    void Handle(MouseButtonEventArg arg);
    void Handle(KeyboardEventArg arg);
    void Handle(RenderingEventArg arg);

    void BindTool(IRenderCanvas* canvas, ITool* tool);
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_MOUSE_SELECTION_
