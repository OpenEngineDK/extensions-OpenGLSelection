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
        class Viewport;
    }
namespace Utils {

class ISceneSelection {
public:
    virtual ~ISceneSelection() {}
    virtual Geometry::Ray Unproject(int x, int y, Display::Viewport& viewport) = 0;    
    virtual Math::Vector<2,float> Project(Math::Vector<3,float> point, Display::Viewport& viewport) = 0;    
    virtual std::list<Scene::ISceneNode*> SelectPoint(int x, int y, 
                                                      Scene::ISceneNode* root, 
                                                      Display::Viewport& viewport,
                                                      Scene::ISceneNode* context = NULL) = 0;
    virtual std::list<Scene::ISceneNode*> SelectRegion(int x1, int y1, int x2, int y2, 
                                                       Scene::ISceneNode* root, 
                                                       Display::Viewport& viewport,
                                                       Scene::ISceneNode* context = NULL) = 0;
    virtual std::list<Scene::ISceneNode*> SelectPointOrtho(int x, int y, 
                                                           Scene::ISceneNode* root, 
                                                           Display::Viewport& viewport,
                                                           Scene::ISceneNode* context = NULL) = 0;
};

}
}
#endif
