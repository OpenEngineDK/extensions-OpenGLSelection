// MouseSelector.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_MOUSE_SELECTOR_
#define _OE_UTILS_MOUSE_SELECTOR_

#include <Core/IListener.h>
#include <Devices/IMouse.h>
#include <Renderers/IRenderer.h>
#include <Math/Vector.h>

// for visitor
#include <Scene/ISceneNodeVisitor.h>
#include <Scene/ISceneNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/TransformationNode.h>


// selection tools
#include <Utils/TransformationSelector.h>
//#include "SelectionHandler.h"
//#include "LineNode.h"

#include <Geometry/Plane.h>


#include <list>
// #include <stack>

namespace OpenEngine {

    // forward declarations
    namespace Display {
        class Viewport;
        class IFrame;
    }
//     namespace Scene {
//         class TransformationNode;
//     }

//     namespace Geometry {
//         class Plane;
//     }
   
//     namespace Devices {
//         class IMouse;
//     }

namespace Utils {

using namespace Devices;
using Core::IListener;
using Renderers::RenderingEventArg;
using Math::Vector;
using Display::Viewport;
using Display::IFrame;

using Scene::TransformationNode;
using Geometry::Plane;
using Scene::ISceneNodeVisitor;
using Scene::ISceneNode;
using Scene::GeometryNode;

using Utils::TransformationSelector;

using std::list;
// using std::stack;

/**
 * Select objects using the mouse.
 *
 * @class MouseSelector MouseSelector.h ts/Selector/MouseSelector.h
 */
class MouseSelector : public IListener<MouseMovedEventArg> 
                    , public IListener<MouseButtonEventArg>
                    , public IListener<RenderingEventArg> 

{
private:
    IFrame& frame;
    IMouse& mouse;
    TransformationSelector& ts;
    float rotationDepth;
    list<TransformationNode*> selection, tmpsel;
    Viewport* activeViewport;
    list<Viewport*> viewports;

    // temporary plane intersection fields
    Plane plane;
    Vector<3,float> startp;
    bool moving;

    //SelectionHandler sh;
    int down_x, down_y;

    void InitMoveSelected(float x, float y, Vector<3,float> startPos);
    void MoveSelected(float x, float y);
    bool IsViewportActive(Viewport* viewport, int x, int y);

    Vector<3,float> GetSelectionPos();
    void MoveSelection(Vector<3,float> dpos);

    // camera handling routines
    // Should not be in this class!!!
    void ZoomIn();
    void ZoomOut();
    void Rotate(float dx, float dy);
    void MoveCamera(float dx, float dy);

public:

    MouseSelector(IFrame& frame, IMouse& mouse, TransformationSelector& ts);
    virtual ~MouseSelector();

    void Handle(MouseMovedEventArg arg);
    void Handle(MouseButtonEventArg arg);
    void Handle(RenderingEventArg arg);

    void AddViewport(Viewport* viewport);
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_MOUSE_SELECTOR_
