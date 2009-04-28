// OpenGL transformation node selector.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/TransformationSelector.h>

#include <Renderers/IRenderer.h>
#include <Renderers/IRenderingView.h>

#include <Geometry/Ray.h>
#include <algorithm>

namespace OpenEngine {

namespace Utils {

using Scene::ISceneNodeVisitor;

using std::min;
using std::max;


TransformationSelector::TransformationSelector (ISceneNode* root, 
                                                IRenderer* r):
    root(root)
    , r(r)
{

}
    
void TransformationSelector::SetRoot(ISceneNode* root) {
    this->root = root;
}

void TransformationSelector::SetRenderer(IRenderer* r) {
    this->r = r;
}


list<TransformationNode*> TransformationSelector::SelectPoint(int x, int y, Viewport& viewport) {
    sr.SetPosition (x,y);
    sr.SetDimension (1,1);
    return sr.Render (r, viewport, root);
}

list<TransformationNode*> TransformationSelector::SelectRegion(int x1, int y1, int x2, int y2, Viewport& viewport) {
    if (x1 == x2 || y1 == y2) return list<TransformationNode*>();
    
    int width  = max(x1,x2) - min(x1,x2);
    int height = max(y1,y2) - min(y1,y2);
    int x      = max(x1,x2) - width/2;
    int y      = max(y1,y2) - height/2;
    sr.SetPosition (x, y);
    sr.SetDimension (width, height);
//     logger.info << "x,y: " << sr.GetPosition() << logger.end;
//     logger.info << "w,h: " << sr.GetDimension() << logger.end;
    return sr.Render(r, viewport, root);
}


Ray TransformationSelector::GetProjectedRay(int x, int y, Viewport& viewport) {
    GLdouble ray_x, ray_y, ray_z;
    GLint viewport_arr[4];
    GLdouble proj[16];
    GLdouble modelview[16];
    IViewingVolume* volume = viewport.GetViewingVolume();
    
    float fproj[16];
    float fmodelview[16];
    viewport.GetDimension().ToArray((int*)viewport_arr);
    volume->GetProjectionMatrix().ToArray(fproj);
    volume->GetViewMatrix().ToArray(fmodelview);

    for (int i = 0; i < 16; i++) {
        proj[i]      = fproj[i];
        modelview[i] = fmodelview[i];
    }

    gluUnProject(x, y, 1.0f, modelview, proj, viewport_arr, &ray_x, &ray_y, &ray_z);
    Vector<3,float> p1(ray_x, ray_y, ray_z);

    return Ray(p1, volume->GetPosition()-p1);

}

}
}
