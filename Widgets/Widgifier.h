// On screen display - Widgify a class
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WIDGETS_WIDGIFIER_
#define _OE_WIDGETS_WIDGIFIER_

#include <Core/IListener.h>
#include <Widgets/CircularSlider.h>
#include <Widgets/Slider.h>
#include <Widgets/Collection.h>
#include <Widgets/Button.h>

#include <string>
#include <list>

namespace OpenEngine {
    namespace Widgets {
        
using Core::IListener;
using std::string;
using std::list;

// mutator structure only for deallocation purposes
class Mutator {
public:
    virtual ~Mutator() {}
};

class Mutators {
private:
    list<Mutator*> mutators;
public:
    void AddMutator(Mutator* m) {
        mutators.push_back(m);
    }
    virtual ~Mutators() {
        for (list<Mutator*>::iterator i = mutators.begin();
             i != mutators.end();
             ++i) {
            delete (*i);
        }
    }
};

#define WIDGET_INIT()                                       \
  private:                                                  \
    Collection _coll;                                       \
    Mutators _mutators;                                     \
  public:                                                   \
    IWidget* GetWidget() { return &_coll; }

#define ADD_WIDGET()                                            \
        _coll.AddWidget(w);                                     \
        _mutators.AddMutator(m);                                      

#define WIDGET_PROPERTY(fname, getfunc, setfunc, objtype, actiontype)  \
        actiontype(fname, getfunc, setfunc, objtype)                   \
        ADD_WIDGET();                                                  \
    }

#define WIDGET_CSLIDER(fname, getfunc, setfunc, objtype, vtype, step)  \
 {                                                                   \
     class _mutator_class: public IListener<ValueChangedEventArg<vtype> >, public Mutator { \
        private:                                                        \
          objtype* obj;                                                 \
          CircularSlider<vtype>* w;                                     \
        public:                                                         \
        _mutator_class(objtype* obj, CircularSlider<vtype>* w)          \
        : obj(obj)                                                      \
        , w(w)                                                          \
        {                                                               \
            w->ValueChangedEvent().Attach(*this);                       \
        }                                                               \
        void Handle(ValueChangedEventArg<vtype> e) {                    \
            obj->setfunc(e.value);                                      \
        }                                                               \
    };                                                                  \
     CircularSlider<vtype>* w = new CircularSlider<vtype>(this->getfunc(),step); \
    w->SetText(#fname);                                                 \
    w->SetDimensions(Vector<2,int>(40,40));                             \
    _mutator_class* m = new _mutator_class(this, w);                    \
        ADD_WIDGET();                                          \
    }

#define WIDGET_SLIDER(fname, getfunc, setfunc, objtype, vtype, low, high) \
 {                                                                   \
     class _mutator_class: public IListener<ValueChangedEventArg<float> >, public Mutator { \
        private:                                                        \
          objtype* obj;                                                 \
          Slider* w;                                                     \
        public:                                                         \
        _mutator_class(objtype* obj, Slider* w)                         \
        : obj(obj)                                                      \
        , w(w)                                                          \
        {                                                               \
            w->ValueChangedEvent().Attach(*this);                       \
        }                                                               \
        void Handle(ValueChangedEventArg<float> e) {                    \
            obj->setfunc((obj->high()-low)*e.value + low);                   \
        }                                                               \
     };                                                                 \
     Slider* w = new Slider();                                          \
     w->SetText(#fname);                                                \
     w->SetDimensions(Vector<2,int>(150,10));                           \
     _mutator_class* m = new _mutator_class(this, w);                   \
    ADD_WIDGET();                                                       \
    }

#define INT_VALUE(fname, getfunc, setfunc, objtype)    \
        VALUE(fname, getfunc, setfunc, objtype, int, 0, 1)

#define FLOAT_VALUE(fname, getfunc, setfunc, objtype)       \
        VALUE(fname, getfunc, setfunc, objtype, float, 0.0, 0.15)

#define VALUE(fname, getfunc, setfunc, objtype, vtype, init, step)   \
 {                                                                   \
    class _mutator_class: public IListener<ValueChangedEventArg<vtype> >, public Mutator { \
        private:                                                        \
          objtype* obj;                                                 \
          CircularSlider<vtype>* w;                                     \
        public:                                                         \
        _mutator_class(objtype* obj, CircularSlider<vtype>* w)          \
        : obj(obj)                                                      \
        , w(w)                                                          \
        {                                                               \
            w->SetValue(obj->getfunc());                                \
            w->ValueChangedEvent().Attach(*this);                       \
        }                                                               \
        void Handle(ValueChangedEventArg<vtype> e) {                    \
            obj->setfunc(e.value);                                      \
        }                                                               \
    };                                                                  \
    CircularSlider<vtype>* w = new CircularSlider<vtype>(init,step);    \
    w->SetText(#fname);                                                 \
    w->SetDimensions(Vector<2,int>(40,40));                             \
    _mutator_class* m = new _mutator_class(this, w);                    \
        
#define BUTTON_STATE(fname, getfunc, setfunc, objtype)                  \
    {                                                                   \
    class _mutator_class: public IListener<StateChangedEventArg>, public Mutator {  \
        private:                                                        \
          objtype* obj;                                                 \
        public:                                                         \
          _mutator_class(objtype* obj): obj(obj) {  }                   \
          void Handle(StateChangedEventArg e) { obj->setfunc(e.state); } \
        };                                                              \
        _mutator_class* m = new _mutator_class(this);                   \
        Collection* w = new Collection(Collection::TOGGLE);             \
        Button* c = new Button();                                       \
        c->SetActive(this->getfunc());                                  \
        c->SetText(#fname);                                             \
        c->SetDimensions(Vector<2,int>(40,40));                         \
        c->StateChangedEvent().Attach(*m);                              \
        w->AddWidget(c);                                               

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_WIDGETS_GUIFIER
