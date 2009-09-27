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

#include <Scene/ISceneNode.h>
#include <Utils/ISceneSelection.h>
#include <Utils/SelectionSet.h>
#include <list>

namespace OpenEngine {
namespace Utils {

/**
 * Pointing Device Abstraction.
 * Used by ITools to determine appropriate actions.
 * @todo find out where to properly place root and select
 * @class PointingDevice PointingDevice.h OpenGLSelection/Utils/PointingDevice.h
 */
class PointingDevice {
public:
    // Pointing device state
    class State {
    public:
        int x, y;
        unsigned int btns;
        unsigned int shifts;
        State(int x, int y, unsigned int btns, unsigned int shifts): 
            x(x) 
            , y(y) 
            , btns(btns)
            , shifts(shifts) 
        {};
    };
    // Pointing device events
    class EventArg {
    public:
        State state;
        ISceneSelection& select;
        Scene::ISceneNode* root;
        PointingDevice& pd;             // primary pointing device 
        std::list<PointingDevice*> pds; // reserved for multiple pointing devices
        Display::Viewport& vp;
        EventArg(
                 ISceneSelection& select, 
                 Scene::ISceneNode* root, 
                 PointingDevice& pd, 
                 Display::Viewport& vp): 
            state(pd.state), 
            select(select), 
            root(root), 
            pd(pd), 
            vp(vp) {}
        virtual ~EventArg() {};
    };
    class MovedEventArg: public EventArg {
    public: 
        int dx, dy;
        MovedEventArg(int dx, 
                      int dy, 
                      ISceneSelection& select, 
                      Scene::ISceneNode* root, 
                      PointingDevice& pd, 
                      Display::Viewport& vp):
            EventArg(select, root, pd, vp), dx(dx), dy(dy) {}
    };
    class PressedEventArg: public EventArg  {
    public: 
        int btn;
        PressedEventArg(int btn, 
                        ISceneSelection& select, 
                        Scene::ISceneNode* root, 
                        PointingDevice& pd, 
                        Display::Viewport& vp): 
            EventArg(select, root, pd, vp), btn(btn) {}
    };
    class ReleasedEventArg: public EventArg  {
    public: 
        int btn;
        ReleasedEventArg(int btn, 
                         ISceneSelection& select, 
                         Scene::ISceneNode* root, 
                         PointingDevice& pd, 
                         Display::Viewport& vp): 
            EventArg(select, root, pd, vp), btn(btn) {}
    };
    State state;
    PointingDevice(): state (State(0, 0, 0x0, 0x0)) {};
    virtual ~PointingDevice() {};
};

} // NS Utils
} // NS OpenEngine
#endif 
