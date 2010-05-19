// Interface for doing scene selection tests.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_INTERFACE_SCENE_SELECTION_
#define _OE_UTILS_INTERFACE_SCENE_SELECTION_

#include <Math/Vector.h>
#include <list>

namespace OpenEngine {
    // forward declarations
    namespace Scene {
        class ISceneNode;
    }
    namespace Geometry {
        class Ray;
    }
    namespace Display {
        class IRenderCanvas;
    }
namespace Utils {

    using Geometry::Ray;
    using Display::IRenderCanvas;
    using Scene::ISceneNode;
    using Math::Vector;

    using std::list;
class ISceneSelection {
public:
    virtual ~ISceneSelection() {}

    virtual Ray Unproject(int x, int y, IRenderCanvas& canvas) = 0;    

    virtual Vector<2,float> Project(Vector<3,float> point, IRenderCanvas& canvas) = 0;    

    virtual list<ISceneNode*> SelectPoint(int x, int y, 
                                          IRenderCanvas& canvas,
                                          ISceneNode* context = NULL) = 0;

    virtual list<ISceneNode*> SelectRegion(int x1, int y1, int x2, int y2, 
                                           IRenderCanvas& canvas,
                                           ISceneNode* context = NULL) = 0;

    virtual list<ISceneNode*> SelectPointOrtho(int x, int y, 
                                               IRenderCanvas& canvas,
                                               ISceneNode* context = NULL) = 0;
};

}
}
#endif
