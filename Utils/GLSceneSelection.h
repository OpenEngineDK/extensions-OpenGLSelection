// OpenGL scene selection tests.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_OPENGL_SCENE_SELECTION_
#define _OE_UTILS_OPENGL_SCENE_SELECTION_

#include <Utils/ISceneSelection.h>
#include <Scene/ISceneNodeVisitor.h>
#include <Scene/SearchTool.h>
#include <Meta/OpenGL.h>
#include <vector>
#include <list>

#define SELECT_BUF_SIZE 64

namespace OpenEngine {
    // forward declarations
    namespace Scene {
        class ISceneNode;
        class TransformationNode;
        class RenderNode;
    }
    namespace Display {
        class Viewport;
        class IFrame;
    }    
    namespace Geometry {
        class Ray;
    }
namespace Utils {

class GLSceneSelection: public ISceneSelection {
private:
    class SelectionRenderer : public Scene::ISceneNodeVisitor {
    private:
        class HitTuple {
        public:
            Scene::ISceneNode* node;
            unsigned int z1, z2;
            HitTuple(Scene::ISceneNode* node, float z1, float z2):
                node(node), z1(z1), z2(z2) {}
        };
        static bool hit_compare(HitTuple& h1, HitTuple& h2) {
            return h1.z1 < h2.z1;
        }
        Scene::SearchTool st;
        unsigned int count;
        std::vector<Scene::ISceneNode*> names; //mapping from name to transformation node
        std::list<HitTuple> hittuples; 
        std::list<Scene::ISceneNode*> hitlist; // z ordered list of selected transformation nodes
        GLuint selectBuf[SELECT_BUF_SIZE];
        GLint  viewport_arr[4];
    public:
        SelectionRenderer();
        std::list<Scene::ISceneNode*> Render(Display::Viewport& viewport, 
                                             Scene::ISceneNode* root,
                                             int x, int y, int width, int height,
                                             Scene::ISceneNode* context);
        void VisitTransformationNode(Scene::TransformationNode* node); 
        void VisitGeometryNode(Scene::GeometryNode* node);
        void VisitRenderNode(Scene::RenderNode* node);
    };
    SelectionRenderer sr;
    Display::IFrame& frame;
public:
    GLSceneSelection(Display::IFrame& frame);
    Geometry::Ray                 Unproject(int x, int y, Display::Viewport& viewport);    
    Math::Vector<2,float>         Project(Math::Vector<3,float> point, Display::Viewport& viewport);    
    std::list<Scene::ISceneNode*> SelectPoint(int x, int y, Scene::ISceneNode* root, 
                                              Display::Viewport& viewport, Scene::ISceneNode* context = NULL);
    std::list<Scene::ISceneNode*> SelectRegion(int x1, int y1, int x2, int y2, Scene::ISceneNode* root, 
                                               Display::Viewport& viewport, Scene::ISceneNode* context = NULL);
    std::list<Scene::ISceneNode*> SelectPointOrtho(int x, int y, Scene::ISceneNode* root, 
                                                   Display::Viewport& viewport, Scene::ISceneNode* context = NULL);
};

}
}
#endif
