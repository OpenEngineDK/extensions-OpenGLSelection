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
template <class T>
class Mutator {
protected:
    T* obj;
public:
    Mutator(T* obj): obj(obj) {}
    void SetObject(T* obj) { this->obj = obj; }
    virtual ~Mutator() {}
};

template <class T>
class Mutators {
private:
    list<Mutator<T>*> mutators;
public:
    void AddMutator(Mutator<T>* m) {
        mutators.push_back(m);
    }
    void SetObject(T* obj) {
        for (typename list<Mutator<T>*>::iterator i = mutators.begin();
             i != mutators.end();
             ++i) {
            (*i)->SetObject(obj);
        }
    };
    virtual ~Mutators() {
        for (typename list<Mutator<T>*>::iterator i = mutators.begin();
             i != mutators.end();
             ++i) {
            delete (*i);
        }
    }
};

#define WIDGET_START(objtype)                               \
  class objtype##Widget : public Widgets::Collection {      \
  private:                                                  \
    typedef objtype thetype;                                \
    thetype* obj;                                           \
    Mutators<thetype> mutators;                             \
  public:                                                   \
    virtual ~objtype##Widget() {}                           \
    void SetObject(thetype* obj) {                          \
        this->obj = obj;                                    \
        mutators.SetObject(obj);                            \
    }                                                       \
    thetype* GetObject() { return obj; }                    \
    objtype##Widget(thetype* obj): obj(obj) {
      
#define WIDGET_CSLIDER(name, getfunc, setfunc, vtype, step)             \
{ class _mutator_class: public IListener<ValueChangedEventArg<vtype> >  \
                      , public Mutator<thetype> {                       \
      private:                                                          \
          CircularSlider<vtype>* w;                                     \
      public:                                                           \
    _mutator_class(thetype* obj, CircularSlider<vtype>* w)              \
        : Mutator<thetype>(obj)                                         \
        , w(w)                                                          \
    {                                                                   \
        w->ValueChangedEvent().Attach(*this);                           \
    }                                                                   \
    void Handle(ValueChangedEventArg<vtype> e) {                        \
        obj->setfunc(e.value);                                          \
    }                                                                   \
      };                                                                \
      CircularSlider<vtype>* w = new CircularSlider<vtype>(obj->getfunc(),step); \
      w->SetText(name);                                                 \
      w->SetDimensions(Vector<2,int>(35,35));                           \
      _mutator_class* m = new _mutator_class(obj, w);                   \
      mutators.AddMutator(m);                                           \
      AddWidget(w);                                                     \
    } 

#define CONST(v) v

#define METHOD(v) obj->v()

#define WIDGET_SLIDER(fname, getfunc, setfunc, lowfunc, low, highfunc, high) \
 {                                                                      \
     class _mutator_class: public IListener<ValueChangedEventArg<float> >, public Mutator<thetype> { \
        private:                                                        \
          Slider* w;                                                    \
        public:                                                         \
        _mutator_class(thetype* obj, Slider* w)                         \
            : Mutator<thetype>(obj)                                     \
        , w(w)                                                          \
        {                                                               \
            w->ValueChangedEvent().Attach(*this);                       \
        }                                                               \
        void Handle(ValueChangedEventArg<float> e) {                    \
            obj->setfunc((highfunc(high)-lowfunc(low))*e.value + lowfunc(low)); \
        }                                                               \
     };                                                                 \
     Slider* w = new Slider();                                          \
     w->SetValue(obj->getfunc()/float(highfunc(high)-lowfunc(low)));    \
     w->SetText(fname);                                                 \
     w->SetDimensions(Vector<2,int>(100,10));                           \
     _mutator_class* m = new _mutator_class(obj, w);                    \
     this->AddWidget(w); \
     mutators.AddMutator(m);\
    }

#define WIDGET_BUTTON(fname, getfunc, setfunc, type)                    \
        type(fname, getfunc, setfunc)

#define TOGGLE(fname, getfunc, setfunc)                                 \
    {                                                                   \
    class _mutator_class: public IListener<ActiveChangedEventArg>, public Mutator<thetype> { \
        public:                                                         \
         _mutator_class(thetype* obj): Mutator<thetype>(obj) {}         \
         void Handle(ActiveChangedEventArg e) { obj->setfunc(e.widget->GetActive()); } \
        };                                                              \
        _mutator_class* m = new _mutator_class(obj);                    \
        Collection* w = new Collection(Collection::TOGGLE);             \
        w->SetBackground(false);                                         \
        w->SetFixed(true);                                              \
        w->SetPadding(Vector<4,int>(4));                                \
        Button* c = new Button();                                       \
        c->SetActive(obj->getfunc());                                   \
        c->SetText(fname);                                              \
        c->SetDimensions(Vector<2,int>(40,40));                         \
        c->ActiveChangedEvent().Attach(*m);                             \
        w->AddWidget(c);                                                \
        AddWidget(w);                                                   \
        mutators.AddMutator(m);                                         \
    }        

#define TRIGGER(fname, getfunc, setfunc)                                \
    {                                                                   \
    class _mutator_class: public IListener<ActiveChangedEventArg>, public Mutator<thetype> { \
        public:                                                         \
         _mutator_class(thetype* obj): Mutator<thetype>(obj) {}         \
         void Handle(ActiveChangedEventArg e) { if (e.widget->GetActive()) obj->setfunc(); } \
        };                                                              \
        Collection* w = new Collection(Collection::SIMPLE);             \
        w->SetBackground(false);                                         \
        w->SetFixed(true);                                              \
        w->SetPadding(Vector<4,int>(4));                                \
        _mutator_class* m = new _mutator_class(obj);                    \
        Button* c = new Button();                                       \
        c->SetText(fname);                                              \
        c->ActiveChangedEvent().Attach(*m);                             \
        w->AddWidget(c);                                                \
        AddWidget(w);                                                   \
        mutators.AddMutator(m);                                         \
    }        


#define WIDGET_STOP()                                                   \
            }                                                           \
        };        

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_WIDGETS_GUIFIER
