// Selection Tool Interface
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/ToolChain.h>
#include <Logging/Logger.h>

#include <Display/IViewingVolume.h>
#include <Renderers/IRenderer.h>

#define dispatchEvent(e)                             \
  for (std::list<ITool*>::iterator i = tools.begin();\
           i != tools.end();                         \
           i++)                                      \
  {                                                  \
      if (!(*i)->Handle(e)) return false;            \
  }                                                  \
  return true                        
 
namespace OpenEngine {
namespace Utils {

using namespace Display;
using namespace Renderers;
       
ToolChain::ToolChain() {}

ToolChain::~ToolChain() {}

bool ToolChain::Handle(PointingDevice::MovedEventArg arg) {
    dispatchEvent(arg);
}

bool ToolChain::Handle(PointingDevice::PressedEventArg arg) {
    dispatchEvent(arg);
}

bool ToolChain::Handle(PointingDevice::ReleasedEventArg arg) {
    dispatchEvent(arg);
}
    
void ToolChain::Render(IViewingVolume& vv, IRenderer& r) {
    //render each tool bottom-up.
    for (list<ITool*>::reverse_iterator i = tools.rbegin();
         i != tools.rend(); 
         i++) {
        (*i)->Render(vv, r);
    }
}

void ToolChain::RenderOrtho(IViewingVolume& vv, Renderers::IRenderer& r) {
    //render each tool bottom-up.
    for (list<ITool*>::reverse_iterator i = tools.rbegin();
         i != tools.rend(); 
         i++) {
        (*i)->RenderOrtho(vv, r);
    }        
}

void ToolChain::PushBackTool(ITool* tool) {
    tools.push_back(tool);
}

} // NS Utils
} // NS OpenEngine

