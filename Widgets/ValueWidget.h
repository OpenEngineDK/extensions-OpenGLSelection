// On screen display - circular slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WIDGETS_VALUE_WIDGET_
#define _OE_WIDGETS_VALUE_WIDGET_

#include <Widgets/IWidget.h>
#include <Core/Event.h>

namespace OpenEngine {
namespace Widgets {

using Core::Event;

template <class T>
class ValueWidget;

template <class T> 
class ValueChangedEventArg {
public:
    T value;
    ValueWidget<T>* widget;
    ValueChangedEventArg(T value, ValueWidget<T>* widget)
        : value(value)
        , widget(widget) {}
};

template <class T>
class ValueWidget: public IWidget {
protected:
    Event<ValueChangedEventArg<T> > e;
    T value, step;
public:
    ValueWidget(T value, T step)
        : IWidget()
        , value(value)
        , step(step)
    {}
    virtual ~ValueWidget() {}
    
    virtual T GetValue() {
        return value;
    }

    virtual void SetValue(T value) {
        this->value = value;
        e.Notify(ValueChangedEventArg<T>(value, this));
    }

    Event<ValueChangedEventArg<T> >& ValueChangedEvent() { return e; }
};

} // NS Widgets
} // NS OpenEngine
#endif //_OE_WIDGETS_VALUE_WIDGET_
