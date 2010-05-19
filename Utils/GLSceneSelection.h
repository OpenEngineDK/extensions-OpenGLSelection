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
        class GeometryNode;
        class RenderNode;
    }
    namespace Display {
        class ICanvas;
        class IViewingVolume;
    }    
    namespace Geometry {
        class Ray;
    }
namespace Utils {

    using Display::ICanvas;
    using Display::IViewingVolume;
    using Geometry::Ray;
    using Scene::ISceneNode;
    using Scene::TransformationNode;
    using Scene::GeometryNode;
    using Scene::RenderNode;
    using Scene::SearchTool;

    using std::vector;
    using std::list;

    class SelectionRenderer : public Scene::ISceneNodeVisitor {
    protected:
        class HitTuple {
        public:
            ISceneNode* node;
            unsigned int z1, z2;
            HitTuple(ISceneNode* node, float z1, float z2):
                node(node), z1(z1), z2(z2) {}
        };
        static bool hit_compare(HitTuple& h1, HitTuple& h2) {
            return h1.z1 < h2.z1;
        }
        SearchTool st;
        unsigned int count;
        vector<ISceneNode*> names; //mapping from name to transformation node
        list<HitTuple> hittuples; 
        list<ISceneNode*> hitlist; // z ordered list of selected transformation nodes
        GLuint selectBuf[SELECT_BUF_SIZE];
        GLint  viewport_arr[4];
    public:
        SelectionRenderer();
        list<ISceneNode*> Render(IViewingVolume& vv,
                                 IRenderCanvas& canvas, 
                                 int x, int y, int width, int height,
                                 ISceneNode* context);
        void VisitTransformationNode(TransformationNode* node); 
        void VisitGeometryNode(GeometryNode* node);
        void VisitRenderNode(RenderNode* node);
    };

class GLSceneSelection: public ISceneSelection {
private:
    SelectionRenderer* sr;
public:
    GLSceneSelection();
    GLSceneSelection(SelectionRenderer* sr);
    ~GLSceneSelection();

    virtual Ray Unproject(int x, int y, IRenderCanvas& canvas);    

    virtual Vector<2,float> Project(Vector<3,float> point, IRenderCanvas& canvas);    

    virtual list<ISceneNode*> SelectPoint(int x, int y, 
                                          IRenderCanvas& canvas,
                                          ISceneNode* context = NULL);

    virtual list<ISceneNode*> SelectRegion(int x1, int y1, int x2, int y2, 
                                           IRenderCanvas& canvas,
                                           ISceneNode* context = NULL);

    virtual list<ISceneNode*> SelectPointOrtho(int x, int y, 
                                               IRenderCanvas& canvas,
                                               ISceneNode* context = NULL);
};

}
}
#endif
