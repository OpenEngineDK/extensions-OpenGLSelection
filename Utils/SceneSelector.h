// OpenGL scene node selector.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_OPENGL_SCENE_SELECTOR_
#define _OE_UTILS_OPENGL_SCENE_SELECTOR_

#include <Scene/ISceneNodeVisitor.h>
#include <Scene/ISceneNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/GeometryNode.h>
#include <Geometry/FaceSet.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Meta/OpenGL.h>

#include <Display/IViewingVolume.h>
#include <Display/Viewport.h>

#include <Logging/Logger.h>

#include <vector>
#include <list>
#include <set>

namespace OpenEngine {

    // forward declarations
    namespace Scene {
        //class ISceneNode;
        //class TransformationNode;
    }

    namespace Renderers {
        class IRenderingView;
        class IRenderer;
    }
    
    namespace Geometry {
        class Ray;
    }

namespace Utils {

using Scene::ISceneNode;
using Scene::ISceneNodeVisitor;
using Scene::TransformationNode;
using Scene::GeometryNode;
using Renderers::IRenderingView;
using Renderers::IRenderer;
using Geometry::FaceSet;
using Geometry::FaceList;
using Geometry::FacePtr;
using Geometry::Ray;
using Math::Vector;
using Math::Matrix;

using Display::IViewingVolume;
using Display::Viewport;

using std::vector;
using std::list;
using std::set;

class SceneSelector {
private:
    class SelectionRenderer : public ISceneNodeVisitor {
    private:
        class HitTuple {
        public:
            ISceneNode* node;
            unsigned int z1, z2;
            HitTuple(ISceneNode* node, float z1, float z2):
                node(node), z1(z1), z2(z2) {}
        };
        
        static bool hit_compare(HitTuple& h1, HitTuple& h2) {
            return h1.z1 > h2.z1;
        }

        unsigned int count;
        vector<ISceneNode*> names; //mapping from name to transformation node
        list<HitTuple> hittuples; 
        list<ISceneNode*> hitlist; // z ordered list of selected transformation nodes
        GLuint selectBuf[1024];
        GLint  viewport_arr[4];
    public:
        SelectionRenderer() {}
  
        list<ISceneNode*> Render(//IRenderer* r, 
                    Viewport& viewport, 
                    ISceneNode* root,
                    int x, int y, int width, int height)
        {
            // setup gl selection
            glSelectBuffer(1024, selectBuf);
            glRenderMode(GL_SELECT);
            
            // initialise naming
            names.clear();
            count = 0;
            glInitNames();
            
            // setup projection matrix 
            viewport.GetDimension().ToArray((int*)viewport_arr);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPickMatrix((GLdouble)x, (GLdouble)(y),
                          width, height, viewport_arr);
            
            IViewingVolume* volume = viewport.GetViewingVolume();
            float arr[16] = {0};
            volume->GetProjectionMatrix().ToArray(arr);
            glMultMatrixf(arr);
            CHECK_FOR_GL_ERROR();
            
            // setup model view matrix
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            // Get the view matrix and apply it
            volume->GetViewMatrix().ToArray(arr);
            glMultMatrixf(arr);
            CHECK_FOR_GL_ERROR();

            // disable expensive gl computations
            glDisable(GL_LIGHTING);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_TEXTURE_2D);
            CHECK_FOR_GL_ERROR();

            // visit scene
            root->Accept(*this);
            
            glFlush();
            GLint hits = glRenderMode(GL_RENDER);
            //logger.info << "glhits: " << hits << logger.end;
            // create an ordered list of selected transformation nodes
            hitlist.clear();
            hittuples.clear();
            unsigned int* ptr = (unsigned int*)selectBuf;
            for (int i = 0; i < hits; i++) {
                int namesInHit = *(ptr++);
                    unsigned int z1 = *ptr++;
                    unsigned int z2 = *ptr;
                    //logger.info << "namesinhit: " << namesInHit << logger.end;
                    
                    // select last name in hit
                    ptr += namesInHit;
                    hittuples.push_back(HitTuple(names[*ptr++], z1, z2));
            }

            hittuples.sort(hit_compare);
            for (list<HitTuple>::iterator itr = hittuples.begin();
                 itr != hittuples.end();
                 itr++){
                hitlist.push_back((*itr).node);
            }
            return hitlist;
        }

        void VisitTransformationNode(TransformationNode* node) {
            glPushName(count++);
            CHECK_FOR_GL_ERROR();
            names.push_back(node);

            // push transformation matrix
            Matrix<4,4,float> m = node->GetTransformationMatrix();
            float f[16];
            m.ToArray(f);
            glPushMatrix();
            CHECK_FOR_GL_ERROR();
            glMultMatrixf(f);
            CHECK_FOR_GL_ERROR();
            // traverse sub nodes
            node->VisitSubNodes(*this);
            // pop transformation matrix
            glPopMatrix();
            glPopName();
            CHECK_FOR_GL_ERROR();
        }
        
        void VisitGeometryNode(GeometryNode* node) {
            glPushName(count++);
            CHECK_FOR_GL_ERROR();
            names.push_back(node);
            
            // Remember last bound texture and shader
            FaceList::iterator itr;
            FaceSet* faces = node->GetFaceSet();
            
            // for each face ...
            for (itr = faces->begin(); itr != faces->end(); itr++) {
                FacePtr f = (*itr);

                glBegin(GL_TRIANGLES);
                // for each vertex ...
                for (int i=0; i<3; i++) {
                    Vector<3,float> v = f->vert[i];
                    glVertex3f(v[0],v[1],v[2]);
                }
                glEnd();
                CHECK_FOR_GL_ERROR();
            
            }
            
            node->VisitSubNodes(*this);
            glPopName();
        }
    };

    SelectionRenderer sr;
public:
    
    list<ISceneNode*> SelectPoint (int x, int y, ISceneNode* root, Viewport& viewport);
    list<ISceneNode*> SelectRegion (int x1, int y1, int x2, int y2, ISceneNode* root, 
                                    Viewport& viewport);
    Ray GetProjectedRay(int x, int y, Viewport& viewport);    
};

}
}
#endif
