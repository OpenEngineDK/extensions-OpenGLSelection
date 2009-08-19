// OpenGL scene selection tests.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/GLSceneSelection.h>

#include <Scene/SearchTool.h>
#include <Scene/ISceneNode.h>
#include <Scene/SceneNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/RenderNode.h>
#include <Display/IViewingVolume.h>
#include <Display/OrthogonalViewingVolume.h>
#include <Display/Viewport.h>
#include <Display/IFrame.h>
#include <Geometry/Ray.h>
#include <algorithm>

#include <Logging/Logger.h>
namespace OpenEngine {

namespace Utils {

using namespace Scene;
using namespace Display;
using namespace std;
using namespace Geometry;

GLSceneSelection::GLSceneSelection(IFrame& frame) : frame(frame) {

}

list<ISceneNode*> GLSceneSelection::SelectPointOrtho(int x, int y, ISceneNode* root, Viewport& viewport, ISceneNode* context) {
    Viewport vp = viewport;
    Vector<4,int> d = vp.GetDimension();
    OrthogonalViewingVolume ortho(1.0f, 2.0f, /*left*/d[0], /*right*/d[0]+d[2], 
                                  /*top*/frame.GetHeight()-d[1]-d[3],
                                  /*bottom*/frame.GetHeight()-d[1]);
    vp.SetViewingVolume(&ortho);
    return sr.Render (vp, root, x, frame.GetHeight()-y, 1, 1, context);
}

    list<ISceneNode*> GLSceneSelection::SelectPoint(int x, int y, ISceneNode* root, Viewport& viewport, ISceneNode* context) {
    Vector<4,int> d = viewport.GetDimension();
    // convert from local viewport coordinates
    return sr.Render (viewport, root, x+d[0], (-1)*(y-d[3]-d[1]), 1, 1, context);
    // return sr.Render (viewport, root, x, frame.GetHeight()-y, 1, 1);
}

list<ISceneNode*> GLSceneSelection::SelectRegion(int x1, int y1, int x2, int y2, 
                                                 ISceneNode* root, Viewport& viewport,
                                                 ISceneNode* context) {
    if (x1 == x2 || y1 == y2) return list<ISceneNode*>();
    int width  = max(x1,x2) - min(x1,x2);
    int height = max(y1,y2) - min(y1,y2);
    int x      = max(x1,x2) - width/2;
    int y      = max(y1,y2) - height/2;
    Vector<4,int> d = viewport.GetDimension();
    // convert from local viewport coordinates
    return sr.Render(viewport, root, x+d[0], (-1)*(y-d[3]-d[1]), width, height, context);
    //return sr.Render(viewport, root, x, frame.GetHeight()-y), width, height);
}


Vector<2,float> GLSceneSelection::Project(Vector<3,float> point, Viewport& viewport) {
    GLdouble px, py, pz;
    GLint vp[4];
    GLdouble proj[16];
    GLdouble model[16];
    IViewingVolume* volume = viewport.GetViewingVolume();
    float fproj[16];
    float fmodel[16];
    viewport.GetDimension().ToArray(vp);
    volume->GetProjectionMatrix().ToArray(fproj);
    volume->GetViewMatrix().ToArray(fmodel);
    for (int i = 0; i < 16; i++) {
        proj[i]      = fproj[i];
        model[i] = fmodel[i];
    }
    gluProject(point[0], point[1], point[2], model, proj, vp, &px, &py, &pz);
    return Vector<2,float>(float(px), frame.GetHeight()-float(py));
}

Ray GLSceneSelection::Unproject(int x, int y, Viewport& viewport) {
    GLdouble rx, ry, rz;
    GLint vp[4];
    GLdouble proj[16];
    GLdouble model[16];
    IViewingVolume* volume = viewport.GetViewingVolume();
    float fproj[16];
    float fmodel[16];
    viewport.GetDimension().ToArray(vp);
    volume->GetProjectionMatrix().ToArray(fproj);
    volume->GetViewMatrix().ToArray(fmodel);
    for (int i = 0; i < 16; i++) {
        proj[i]      = fproj[i];
        model[i] = fmodel[i];
    }
    gluUnProject(x, frame.GetHeight()-y, 0.0f, model, proj, vp, &rx, &ry, &rz);
    Vector<3,float> p1(rx, ry, rz);
    return Ray(p1, volume->GetPosition()-p1);
}

GLSceneSelection::SelectionRenderer::SelectionRenderer() {

}
  
list<ISceneNode*> GLSceneSelection::SelectionRenderer::Render(Viewport& viewport, 
                                                              ISceneNode* root,
                                                              int x, int y, int width, int height,
                                                              ISceneNode* context) {
    // setup gl selection
    glSelectBuffer(SELECT_BUF_SIZE, selectBuf);
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

    // apply ancestor matrices
    list<TransformationNode*> nodes;
    if (context) {
        //*** UGLY CODE BEGIN ****
        // THIS CODE CAN BE AVOIDED IF SEARCHTOOL:ANCESTORTRANSFORMATIONNODES 
        // WOULD INCLUDE THE STARTING NODE. WE SHOULD NOT MODIFY THE CONTEXT NODE
        // BY ADDING SOMETHING AND REMOVING IT AGAIN!
        //@todo FIX SEARCHTOOL
        SearchTool st;
        ISceneNode* dummyNode = new SceneNode();
        context->AddNode(dummyNode);
        nodes = st.AncestorTransformationNodes(dummyNode);
        context->RemoveNode(dummyNode);
        delete dummyNode;
        /// ****** UGLY CODE END ******
        for (list<TransformationNode*>::reverse_iterator tn = nodes.rbegin();
             tn != nodes.rend();
             tn++) {
            float f[16];
            (*tn)->GetTransformationMatrix().ToArray(f);
            glPushMatrix();
            CHECK_FOR_GL_ERROR();
            glMultMatrixf(f);
            CHECK_FOR_GL_ERROR();
        }
    }
    // visit scene
    root->Accept(*this);
    for (unsigned int i = 0; i < nodes.size(); i++)
        glPopMatrix();
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

void GLSceneSelection::SelectionRenderer::VisitTransformationNode(TransformationNode* node) {
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

void GLSceneSelection::SelectionRenderer::VisitGeometryNode(GeometryNode* node) {
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

void GLSceneSelection::SelectionRenderer::VisitRenderNode(RenderNode* node) {
    glPushName(count++);
    CHECK_FOR_GL_ERROR();
    names.push_back(node);
    node->Apply(NULL);
    node->VisitSubNodes(*this);
    glPopName();
}

}
}
