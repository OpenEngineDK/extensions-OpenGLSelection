// Pointing Device abstraction
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_POINTING_DEVICE_
#define _OE_UTILS_POINTING_DEVICE_

#define PD_EVENT_MOVED    0x0
#define PD_EVENT_PRESSED  0x1
#define PD_EVENT_RELEASED 0x2

#include <Scene/ISceneNode.h>
#include <Utils/ISceneSelection.h>
#include <Utils/SelectionSet.h>
#include <list>

namespace OpenEngine {
namespace Utils {

/**
 * Pointing Device Abstraction.
 * Used by ITool's to determine appropriate actions.
 * @todo find out where to properly place root and select
 * @class PointingDevice PointingDevice.h OpenGLSelection/Utils/PointingDevice.h
 */
class PointingDevice {
public:
    // Pointing device state
    class State {
    public:
        State(int x, int y, bool btn1down, bool btn2down, bool btn3down): x(x) 
                                                                        , y(y) 
                                                                        , btn1down(btn1down)
                                                                        , btn2down(btn2down)
                                                                        , btn3down(btn3down) 
        {};
        int x, y;
        bool btn1down, btn2down, btn3down;
    };
    // Pointing device events
    class EventArg {
    private:
        int type;
    public:
        State state;
        ISceneSelection& select;
        Scene::ISceneNode* root;
        SelectionSet<Scene::ISceneNode>& sset;
        PointingDevice& pd;             // primary pointing device 
        std::list<PointingDevice*> pds; // reserved for multiple pointing devices
        Display::Viewport& vp;
        EventArg(int type, 
                 ISceneSelection& select, 
                 Scene::ISceneNode* root, 
                 SelectionSet<Scene::ISceneNode>& sset, 
                 PointingDevice& pd, 
                 Display::Viewport& vp): 
            type(type), 
            state(pd.state), 
            select(select), 
            root(root), 
            sset(sset), 
            pd(pd), 
            vp(vp) {}
        virtual ~EventArg() {};
        int EventType() {return type;}
    };
    class MovedEventArg: public EventArg {
    public: 
        int dx, dy;
        MovedEventArg(int dx, 
                      int dy, 
                      ISceneSelection& select, 
                      Scene::ISceneNode* root, 
                      SelectionSet<Scene::ISceneNode>& sset,
                      PointingDevice& pd, 
                      Display::Viewport& vp):
            EventArg(PD_EVENT_MOVED, select, root, sset, pd, vp), dx(dx), dy(dy) {}
    };
    class PressedEventArg: public EventArg  {
    public: 
        int btn;
        PressedEventArg(int btn, 
                        ISceneSelection& select, 
                        Scene::ISceneNode* root, 
                        SelectionSet<Scene::ISceneNode>& sset,
                        PointingDevice& pd, 
                        Display::Viewport& vp): 
            EventArg(PD_EVENT_PRESSED, select, root, sset, pd, vp), btn(btn) {}
    };
    class ReleasedEventArg: public EventArg  {
    public: 
        int btn;
        ReleasedEventArg(int btn, 
                         ISceneSelection& select, 
                         Scene::ISceneNode* root, 
                         SelectionSet<Scene::ISceneNode>& sset,
                         PointingDevice& pd, 
                         Display::Viewport& vp): 
            EventArg(PD_EVENT_RELEASED, select, root, sset, pd, vp), btn(btn) {}
    };
    State state;
    // ISceneSelection& select;
    // Scene::ISceneNode* root;
    // SelectionSet<Scene::ISceneNode>& sset;
    PointingDevice(/*ISceneSelection& select,
                   Scene::ISceneNode* root,
                   SelectionSet<Scene::ISceneNode>& sset*/): state (State(0, 0, false, false, false))
                                                             /*, select(select)
                                                               , root(root)
                                                               , sset(sset) */ {};
    virtual ~PointingDevice() {};
};

} // NS Utils
} // NS OpenEngine
#endif 
