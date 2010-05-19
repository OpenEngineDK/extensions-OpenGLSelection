// OpenGL scene selection tests.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/GLSceneSelection.h>

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

GLSceneSelection::GLSceneSelection() 
    : sr(new SelectionRenderer())
{
    
}

GLSceneSelection::GLSceneSelection(SelectionRenderer* sr) 
    : sr(sr)
{

}

GLSceneSelection::~GLSceneSelection() {
    delete sr;
}

list<ISceneNode*> GLSceneSelection::SelectPointOrtho(int x, int y, IRenderCanvas& canvas, ISceneNode* context) {
    OrthogonalViewingVolume ortho(1.0f, 2.0f, 0, canvas.GetWidth(),
                                  0, canvas.GetHeight());
    return sr->Render (ortho, canvas, x, canvas.GetHeight()-y, 1, 1, context);
}

list<ISceneNode*> GLSceneSelection::SelectPoint(int x, int y, IRenderCanvas& canvas, ISceneNode* context) {
    return sr->Render (*canvas.GetViewingVolume(), canvas, x, canvas.GetHeight()-y, 1, 1, context);
}

list<ISceneNode*> GLSceneSelection::SelectRegion(int x1, int y1, int x2, int y2, 
                                                 IRenderCanvas& canvas,
                                                 ISceneNode* context) {
    if (x1 == x2 || y1 == y2) return list<ISceneNode*>();
    int width  = max(x1,x2) - min(x1,x2);
    int height = max(y1,y2) - min(y1,y2);
    int x      = max(x1,x2) - width/2;
    int y      = max(y1,y2) - height/2;
    return sr->Render(*canvas.GetViewingVolume(), canvas, x, canvas.GetHeight()-y, width, height, context);
}


Vector<2,float> GLSceneSelection::Project(Vector<3,float> point, IRenderCanvas& canvas) {
    GLdouble px, py, pz;
    GLint vp[4] = {0, 0, canvas.GetWidth(), canvas.GetHeight()};
    GLdouble proj[16];
    GLdouble model[16];
    IViewingVolume* volume = canvas.GetViewingVolume();
    // conversion from float to double
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            proj[i*4+j]      = volume->GetProjectionMatrix()(i,j);
            model[i*4+j]     = volume->GetViewMatrix()(i, j);
        }
    }
    gluProject(point[0], point[1], point[2], model, proj, vp, &px, &py, &pz);
    return Vector<2,float>(float(px), float(canvas.GetHeight())-float(py));
}

Ray GLSceneSelection::Unproject(int x, int y, IRenderCanvas& canvas) {
    GLdouble rx, ry, rz;
    GLint vp[4] = {0, 0, canvas.GetWidth(), canvas.GetHeight()};
    GLdouble proj[16];
    GLdouble model[16];
    IViewingVolume* volume = canvas.GetViewingVolume();
    // conversion from float to double
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            proj[i*4+j]      = volume->GetProjectionMatrix()(i,j);
            model[i*4+j]     = volume->GetViewMatrix()(i, j);
        }
    }
    gluUnProject(x, canvas.GetHeight()-y, 0.0f, model, proj, vp, &rx, &ry, &rz);
    Vector<3,float> p1(rx, ry, rz);
    return Ray(volume->GetPosition(), p1-volume->GetPosition());
}

SelectionRenderer::SelectionRenderer() {

}
  
list<ISceneNode*> SelectionRenderer::Render(IViewingVolume& vv,
                                            IRenderCanvas& canvas,
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
    Vector<4,int> dims(0, 0, canvas.GetWidth(), canvas.GetHeight());
    dims.ToArray((int*)viewport_arr);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // logger.info << "render x: " << x << " y: " << y << logger.end;
    gluPickMatrix((GLdouble)x, (GLdouble)(y),
                  (GLdouble)width, (GLdouble)height, viewport_arr);
    IViewingVolume& volume = vv;
    float arr[16] = {0};
    volume.GetProjectionMatrix().ToArray(arr);
    glMultMatrixf(arr);
    CHECK_FOR_GL_ERROR();
    // setup model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Get the view matrix and apply it
    volume.GetViewMatrix().ToArray(arr);
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
        nodes = st.AncestorTransformationNodes(context,true);
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
    canvas.GetScene()->Accept(*this);
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
         itr++)
        {
            logger.info << "hit" << logger.end;

        hitlist.push_back((*itr).node);
    }
    return hitlist;
}

void SelectionRenderer::VisitTransformationNode(TransformationNode* node) {
    //    glPushName(count++);
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
    //    glPopName();
    CHECK_FOR_GL_ERROR();
}

void SelectionRenderer::VisitGeometryNode(GeometryNode* node) {
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

void SelectionRenderer::VisitRenderNode(RenderNode* node) {
    glPushName(count++);
    CHECK_FOR_GL_ERROR();
    names.push_back(node);
    // node->Apply(NULL);
    node->VisitSubNodes(*this);
    glPopName();
}

}
}
