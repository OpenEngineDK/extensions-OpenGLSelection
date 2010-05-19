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

#include <Display/IRenderCanvas.h>
#include <list>

namespace OpenEngine {
namespace Utils {
    using Display::IRenderCanvas;
    using std::list;

/**
 * Pointing Device Abstraction.
 * Used by ITools to determine appropriate actions.
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
        PointingDevice& pd;             // primary pointing device 
        list<PointingDevice*> pds; // reserved for multiple pointing devices
        IRenderCanvas& canvas;
        EventArg(
                 PointingDevice& pd, 
                 IRenderCanvas& canvas 
                 ): 
            state(pd.state), 
            pd(pd), 
            canvas(canvas) 
        {}
        virtual ~EventArg() {};
    };
    class MovedEventArg: public EventArg {
    public: 
        int dx, dy;
        MovedEventArg(int dx, 
                      int dy, 
                      PointingDevice& pd, 
                      IRenderCanvas& canvas
                      ):
            EventArg(pd, canvas), dx(dx), dy(dy) {}
    };
    class PressedEventArg: public EventArg  {
    public: 
        int btn;
        PressedEventArg(int btn, 
                        PointingDevice& pd, 
                        IRenderCanvas& canvas
                        ): 
            EventArg(pd, canvas), btn(btn) {}
    };
    class ReleasedEventArg: public EventArg  {
    public: 
        int btn;
        ReleasedEventArg(int btn, 
                         PointingDevice& pd, 
                         IRenderCanvas& canvas
                         ): 
            EventArg(pd, canvas), btn(btn) {}
    };
    State state;
    PointingDevice(): state (State(0, 0, 0x0, 0x0)) {};
    virtual ~PointingDevice() {};
};

} // NS Utils
} // NS OpenEngine
#endif 
