// ITool - Transformation Tool
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Utils/TransformationTool.h>
#include <Utils/ISceneSelection.h>
#include <Utils/SelectionSet.h>
#include <Renderers/IRenderer.h>
#include <Scene/ISceneNode.h>
#include <Scene/SceneNode.h>
#include <Scene/TransformationNode.h>
#include <Display/OrthogonalViewingVolume.h>
#include <Display/IFrame.h>
#include <Display/IViewingVolume.h>
#include <Geometry/Tests.h>
#include <Geometry/Ray.h>
#include <Resources/ITextureResource.h>
#include <Resources/DirectoryManager.h>
#include <Resources/ResourceManager.h>
#include <list>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Utils {
    
using namespace Renderers;
using namespace Display;
using namespace Scene;
using namespace Geometry;
using namespace Resources;
using namespace std;

TransformationTool::TransformationTool(): 
      transformation(&translation)
    , buttons(new SceneNode())
    , traBtn(new Button(25, 25,16))
    , rotBtn(new Button(25, 25+38,16))
    , sclBtn(new Button(25, 25+38+38,16)) {

    buttons->AddNode(traBtn);
    buttons->AddNode(rotBtn);
    buttons->AddNode(sclBtn);

    ITextureResourcePtr tra = 
        ResourceManager<ITextureResource>::Create("Buttons/translate.png");
    ITextureResourcePtr rot = 
        ResourceManager<ITextureResource>::Create("Buttons/rotate.png");
    ITextureResourcePtr scl = 
        ResourceManager<ITextureResource>::Create("Buttons/scale.png");
    // Vector<4,float> fc(.125f,  .1367f, .3281f, 1.0f);
    Vector<4,float> bc(1.0, 1.0, 1.0, 0.85f);
    Vector<4,float> fc(202.0f/256.0f,  255.0f/256.0f, 97.0f/256.0f, 1.0f);

    traBtn->selected = true;
    traBtn->bg = tra;
    traBtn->bgc = bc;
    traBtn->fgc = fc;
    rotBtn->bg = rot;
    rotBtn->bgc = bc;
    rotBtn->fgc = fc;
    sclBtn->bg = scl;
    sclBtn->bgc = bc;
    sclBtn->fgc = fc;
}

TransformationTool::~TransformationTool() {
    delete buttons;
}

bool TransformationTool::Handle(PointingDevice::MovedEventArg arg) {
    if (selection.empty()) return true;
    if (transformation->Transform(arg.state.x, arg.state.y, arg.dx, arg.dy, arg.select, selection, arg.vp))
        return false;
    //@todo: don't use projection but just check bounding rectangle.
    traBtn->focus = false;
    rotBtn->focus = false;
    sclBtn->focus = false;
    list<ISceneNode*> btn = arg.select.SelectPointOrtho(arg.state.x,
                                                        arg.state.y,
                                                        buttons, 
                                                        arg.vp);
    if (!btn.empty()) {
        if (btn.front() == traBtn) {
            traBtn->focus = true;
        }
        else if (btn.front() == rotBtn) {
            rotBtn->focus = true;
        }
        else if (btn.front() == sclBtn) {
            sclBtn->focus = true;
        }
    }
    return false;
}

bool TransformationTool::Handle(PointingDevice::PressedEventArg arg) {
    if (selection.empty()) return true;
    switch (arg.btn) {
    case 1:
        if (traBtn->focus) return false;
        if (rotBtn->focus) return false;
        if (sclBtn->focus) return false;
        if (transformation->GrabWidget(arg.state.x, arg.state.y, arg.select, selection, arg.vp))
            return false;
        // check for selection hit
        list<ISceneNode*> ns = arg.select.SelectPoint(arg.state.x,
                                                  arg.state.y,
                                                  arg.root, 
                                                  arg.vp);
        if (!ns.empty()) {
            TransformationNode* t = search.AncestorTransformationNode(ns.front());
            if (t && transformation->GrabSelection(arg.state.x, arg.state.y, arg.select, t, selection, arg.vp))
                return false;
        }
    }
    return true;
}

bool TransformationTool::Handle(PointingDevice::ReleasedEventArg arg) {
    switch (arg.btn) {
    case 1: 
        if (transformation->Reset())
            return false;
        
        //todo: @ ugly hacked up code
        if (traBtn->focus) {
            traBtn->selected = true;
            rotBtn->selected = false;
            sclBtn->selected = false;
            transformation = &translation;
            return false;
        }
        if (rotBtn->focus) {
            rotBtn->selected = true;
            traBtn->selected = false;
            sclBtn->selected = false;
            transformation = &rotation;
            return false;
        }
        if (sclBtn->focus) {
            sclBtn->selected = true;
            traBtn->selected = false;
            rotBtn->selected = false;
            transformation = &scaling;
            return false;
        }
    };
    return true;
}

void TransformationTool::Handle(SelectionSet<ISceneNode>::ChangedEventArg arg) {
    // @todo: consider which strategy to use (first common transformation node?)
    // For each element in SelectionSet we see if itself or its parent
    // is a transformation node and add it to our private list.
    selection.clear();
    //ISceneNode* dummy = new SceneNode();
    for (set<ISceneNode*>::iterator itr = arg.selection.begin();
         itr != arg.selection.end();
         itr++) {
        ISceneNode* node = (*itr);
        ISceneNode* parent = node->GetParent();
        if (parent) {
            // check if selected node is already a transformation node
            TransformationNode* tn = NULL;
            list<TransformationNode*> nodes = search.ChildTransformationNodes(parent);
            for (list<TransformationNode*>::iterator itr = nodes.begin();
                 itr != nodes.end(); 
                 itr++) {
                if (*itr == node) {
                    tn = *itr;
                    break;
                }
            }
            if (tn) {
                selection.insert(tn);
                continue;
            }
            // check if parent is a transformation node
            ISceneNode* parentparent = parent->GetParent();
            if (parentparent) {
                nodes = search.ChildTransformationNodes(parentparent);
                for (list<TransformationNode*>::iterator itr = nodes.begin();
                     itr != nodes.end(); 
                     itr++) {
                    if (*itr == parent) {
                        tn = *itr;
                        break;
                    }
                }
                if (tn) 
                    selection.insert(tn);
            }
        }
    }
}
    
void TransformationTool::Render(IViewingVolume& vv, IRenderer& r) {
    if (selection.empty()) return;
    glPushAttrib(GL_LIGHTING_BIT);
    glPushAttrib(GL_DEPTH_BUFFER_BIT);
    glPushAttrib(GL_ENABLE_BIT);
    glColor4f(0.0,0.0,1.0,0.3);
    glLineWidth(2.0);
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_MODELVIEW);
    // outline selected objects
    // @todo optimize by reusing shared transformations
    for (set<TransformationNode*>::iterator itr = selection.begin();
         itr != selection.end(); 
         itr++) {
        list<TransformationNode*> ts = search.AncestorTransformationNodes(*itr);
        for (list<TransformationNode*>::reverse_iterator t = ts.rbegin();
             t != ts.rend();
             t++) {
            ApplyTransformationNode(*t);
        }
        glScalef(1.001, 1.001, 1.001);
        (*itr)->Accept(this->sr);
        for (unsigned int i = 0; i < ts.size(); i++) 
            glPopMatrix();
    }
    glDisable(GL_DEPTH_TEST);
    transformation->RenderWidget(vv, *selection.begin());
    glDisable(GL_BLEND);
    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
}
    
void TransformationTool::RenderOrtho(IViewingVolume& vv, Renderers::IRenderer& r) {
    if (selection.empty()) return;
    glPushAttrib(GL_LIGHTING_BIT);
    glPushAttrib(GL_DEPTH_BUFFER_BIT);
    glPushAttrib(GL_ENABLE_BIT);
    //@todo: find a better tex-loading strategy
    if (traBtn->bg != NULL && traBtn->bg->GetID() == 0)
        r.LoadTexture(traBtn->bg);
    if (rotBtn->bg != NULL && rotBtn->bg->GetID() == 0)
        r.LoadTexture(rotBtn->bg);
    if (sclBtn->bg != NULL && sclBtn->bg->GetID() == 0)
        r.LoadTexture(sclBtn->bg);
    traBtn->Apply(NULL);
    rotBtn->Apply(NULL);
    sclBtn->Apply(NULL);
    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
}

TransformationTool::Button::Button(float posx, float posy, float size)
    : posx(posx)
    , posy(posy)
    , size(size)
    , selected(false)
    , focus(false) {}

void TransformationTool::Button::Apply(Renderers::IRenderingView* rv) {
    float x, y;
    float offs = 2;
    float col[4];
    if (selected) {
        fgc.ToArray(col);
    }
    else {
        bgc.ToArray(col);
    }
    if (focus) {
        x = posx-offs;
        y = posy-offs;
    }
    else {
        x = posx;
        y = posy;
    }
    if (bg == NULL) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        CHECK_FOR_GL_ERROR();
    }
    else {
        glBindTexture(GL_TEXTURE_2D, bg->GetID());
        glEnable(GL_TEXTURE_2D);
    }
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4fv(col);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(x-size, y-size, -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(x-size, y+size, -1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(x+size, y+size, -1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(x+size, y-size, -1.0);
    glEnd();    
}

TransformationTool::AxisWidget::Axis::Axis(float rot, 
                                           Vector<3,float> rotaxis, 
                                           Vector<3,float> dir, 
                                           Vector<4,float> colr): 
    quadr(gluNewQuadric()),
    rot(rot), rotaxis(rotaxis), dir(dir), colr(colr) {
    gluQuadricNormals(quadr, GLU_SMOOTH);
}

TransformationTool::AxisWidget::Axis::~Axis() {
    gluDeleteQuadric(quadr);
} 


TransformationTool::AxisWidget::XAxis::XAxis(): 
    Axis(90.0f, Vector<3,float>(0.0,1.0,0.0), Vector<3,float>(1.0,0.0,0.0), Vector<4,float> (1.0,0.0,0.0,0.8))
{
}

TransformationTool::AxisWidget::YAxis::YAxis(): 
    Axis(-90.0f, Vector<3,float>(1.0,0.0,0.0), Vector<3,float>(0.0,1.0,0.0), Vector<4,float> (0.0,1.0,0.0,0.8))
{
}

TransformationTool::AxisWidget::ZAxis::ZAxis(): 
    Axis(0.0f, Vector<3,float>(0.0), Vector<3,float>(0.0,0.0,1.0), Vector<4,float> (0.0,0.0,1.0,0.8))
{
}

void TransformationTool::AxisWidget::Axis::Apply(Renderers::IRenderingView* rv) {
    glPushMatrix();
    glColor4f(colr[0], colr[1], colr[2], colr[3]);
    glPushMatrix();
    glRotatef(rot, rotaxis[0], rotaxis[1], rotaxis[2]);
    gluCylinder(quadr, 0.25, 0.40, 10, 10, 10);
    glPopMatrix();
    glTranslatef(10*dir[0], 10*dir[1], 10*dir[2]);
    glRotatef(rot, rotaxis[0], rotaxis[1], rotaxis[2]);
    gluCylinder(quadr, 1.0, 0.0, 3.0, 20, 10);
    glPopMatrix();
}

TransformationTool::AxisWidget::AxisWidget(): 
    axes(new SceneNode())
    , xaxis(new XAxis())
    , yaxis(new YAxis())
    , zaxis(new ZAxis())
{
    axes->AddNode(xaxis);
    axes->AddNode(yaxis);
    axes->AddNode(zaxis);
}

TransformationTool::AxisWidget::~AxisWidget() {
    delete axes;
}

void TransformationTool::AxisWidget::Render(IViewingVolume& vv, ISceneNode* context, bool rotate) {
    SearchTool st;
    Vector<3,float> p;
    Quaternion<float> q;
    TransformationNode* t = st.AncestorTransformationNode(context, true);
    if (t) t->GetAccumulatedTransformations(&p, &q); 
    TransformationNode tmpn;
    tmpn.SetPosition(p);
    if (rotate) 
        tmpn.SetRotation(q);
    float size = (p-vv.GetPosition()).GetLength()/100;
    tmpn.Scale(size, size, size);
    float f[16];
    tmpn.GetTransformationMatrix().ToArray(f);
    glPushMatrix();
    CHECK_FOR_GL_ERROR();
    glMultMatrixf(f);
    CHECK_FOR_GL_ERROR();
    xaxis->Apply(NULL);
    yaxis->Apply(NULL);
    zaxis->Apply(NULL);
    glPopMatrix();
}

bool TransformationTool::AxisWidget::GrabAxis(int x, int y, int offset, ISceneSelection& select, Scene::ISceneNode* context, Display::Viewport& vp, Vector<3,float>& dir, bool rotate) {
    SearchTool st;
    Vector<3,float> p;
    Quaternion<float> q;
    TransformationNode* t = st.AncestorTransformationNode(context, true);
    if (t) t->GetAccumulatedTransformations(&p, &q); 
    TransformationNode tmpn;
    tmpn.SetPosition(p);
    if (rotate) 
        tmpn.SetRotation(q);
    IViewingVolume* vv = vp.GetViewingVolume();
    float size = (p-vv->GetPosition()).GetLength()/100;
    tmpn.Scale(size, size, size);
    list<ISceneNode*> s = select.SelectRegion(x - offset,
                                              y - offset,
                                              x + offset,
                                              y + offset,
                                              axes, 
                                              vp,
                                              &tmpn);
    // check for arrow hit
    if (s.empty()) return false;
    Axis* axis = NULL;
    if (s.front() == xaxis) {
        axis = xaxis;
    }
    else if (s.front() == yaxis) { 
        axis = yaxis;
    }
    else if (s.front() == zaxis) {
        axis = zaxis;
    }
    if (!axis) return false;
    dir = axis->dir;
    return true;
}

void TransformationTool::SelectionRenderer::VisitGeometryNode(Scene::GeometryNode* node) {
    FaceList::iterator itr;
    FaceSet* faces = node->GetFaceSet();
    if (faces == NULL) return;
    // for each face ...
    for (itr = faces->begin(); itr != faces->end(); itr++) {
        FacePtr f = (*itr);
        Vector<3,float> v0 = f->vert[0];
        Vector<3,float> v1 = f->vert[1];
        Vector<3,float> v2 = f->vert[2];
        glBegin(GL_LINE_STRIP);
        glVertex3f(v0[0],v0[1],v0[2]);
        glVertex3f(v1[0],v1[1],v1[2]);
        glVertex3f(v2[0],v2[1],v2[2]);
        glVertex3f(v0[0],v0[1],v0[2]);
        glEnd();
    }
    CHECK_FOR_GL_ERROR();
}

void TransformationTool::SelectionRenderer::VisitTransformationNode(TransformationNode* node) {
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
    CHECK_FOR_GL_ERROR();
}

inline void TransformationTool::ApplyTransformationNode(TransformationNode* node) {
    float f[16];
    node->GetTransformationMatrix().ToArray(f);
    glPushMatrix();
    CHECK_FOR_GL_ERROR();
    glMultMatrixf(f);
    CHECK_FOR_GL_ERROR();
}

TransformationTool::TranslationStrategy::TranslationStrategy(): 
    axisgrabbed(false) 
    , plane(Vector<3,float>(1), Vector<3,float>(1))
    , t(NULL)
{
}

TransformationTool::TranslationStrategy::~TranslationStrategy() {
} 

void TransformationTool::TranslationStrategy::UpdateTransformationNodes(float x, float y, 
                                                                        Viewport& vp,
                                                                        ISceneSelection& ssel,
                                                                        set<TransformationNode*> selection) {
    //move selected object according to plane and ray intersection
    Vector<3,float> pos, dp;
    Tests::Intersects(plane, ssel.Unproject(x, y, vp), &pos);
    dp = pos-oldp;
    for (set<TransformationNode*>::iterator itr = selection.begin();
         itr != selection.end();
         itr++) {
        Vector<3,float> p;
        Quaternion<float> q;
        (*itr)->GetAccumulatedTransformations(&p, &q);
        q.Normalize();
        p = q.GetInverse().RotateVector(dp);
       (*itr)->Move(p[0], p[1], p[2]);
    }
    oldp = pos;
}

void TransformationTool::TranslationStrategy::CalcMovePlane(float x, float y, 
                                       Vector<3,float> pos, 
                                       Viewport& vp,
                                       ISceneSelection& ssel) {
    // construct plane and calc initial ray intersection
    plane = Plane(vp.GetViewingVolume()->
                  GetDirection().RotateVector(Vector<3,float>(0.0,0.0,1.0)),
                  pos);
    Tests::Intersects(plane, ssel.Unproject(x, y, vp), &oldp);
}

bool TransformationTool::TranslationStrategy::Transform(int x, 
                                                        int y,
                                                        int dx,
                                                        int dy,
                                                        ISceneSelection& select,
                                                        set<TransformationNode*> selection,
                                                        Viewport& vp) {
    if (axisgrabbed) {
        Vector<2,float> dmouse = Vector<2,float>(x, y) - mouse;
        mouse = Vector<2,float>(x, y);
        Vector<3,float> d = dir.GetNormalize() * (axisproj * dmouse) * ratio;
        Vector<3,float> p;
        Quaternion<float> q;
        for (set<TransformationNode*>::iterator itr = selection.begin();
             itr != selection.end();
             itr++) {
            (*itr)->GetAccumulatedTransformations(&p, &q);
            q.Normalize();
            p = q.GetInverse().RotateVector(d);
            (*itr)->Move(p[0], p[1], p[2]);
        }
        return true;
    }
    if (t) {
        UpdateTransformationNodes(x, y, vp, select, selection);
        return true;
    }
    return false;
}

bool TransformationTool::TranslationStrategy::GrabWidget(int x, 
                                                         int y, 
                                                         ISceneSelection& select,
                                                         set<TransformationNode*> selection,
                                                         Viewport& vp) {
    int offset = 2;
    Vector<3,float> p;
    Quaternion<float> q;
    (*selection.begin())->GetAccumulatedTransformations(&p, &q);
    axisgrabbed = axiswidget.GrabAxis(x, y, offset, select, *selection.begin(), vp, dir, false);
    if (!axisgrabbed)
        return false;    
    axisproj = select.Project(p + dir, vp) 
        - select.Project(p, vp); 
    if (axisproj.IsZero()) 
        //@todo: what to do with this value???
        axisproj = Vector<2,float>(0.0,3.0);
    //p = dir;
    ratio = (dir.GetLength())/(axisproj.GetLength());
    mouse = Vector<2,float>(x, y);
    axisproj.Normalize();
    return true;
}
    
bool TransformationTool::TranslationStrategy::GrabSelection(int x, 
                                                            int y, 
                                                            ISceneSelection& select, 
                                                            TransformationNode* node,
                                                            set<TransformationNode*> selection,
                                                            Viewport& vp) {
    // check for selection hit
    set<TransformationNode*>::iterator i = selection.find(node);
    if (i != selection.end()) {
        t = node;
        CalcMovePlane(x, y, t->GetPosition(), vp, select);
        return true;
    }
    return false;
}

void TransformationTool::TranslationStrategy::RenderWidget(IViewingVolume& vv, ISceneNode* context) {
    axiswidget.Render(vv, context, false);
}

bool TransformationTool::TranslationStrategy::Reset() {
    if (axisgrabbed) {
        axisgrabbed = false;
        return true;
    }
    if (t) {
        t = NULL;
        return true;
    }
    return false;
}

TransformationTool::RotationStrategy::RotationStrategy(): 
    axisgrabbed(false) 
{
}

TransformationTool::RotationStrategy::~RotationStrategy() {
} 


bool TransformationTool::RotationStrategy::Transform(int x, 
                                                     int y,
                                                     int dx,
                                                     int dy,
                                                     ISceneSelection& select,
                                                     set<TransformationNode*> selection,
                                                     Viewport& vp) {
    if (axisgrabbed) {
        Vector<2,float> dmouse = Vector<2,float>(x, y) - mouse;
        mouse = Vector<2,float>(x, y);
        float sign = 1.0f;
        if (axisproj * dmouse < 0) 
            sign = -1.0f;
        Vector<3,float> d = sign * dir.GetNormalize();
        Vector<3,float> p;
        Quaternion<float> q;
        (*selection.begin())->GetAccumulatedTransformations(&p, &q);
        q.Normalize();
        d = q.RotateVector(d);
        for (set<TransformationNode*>::iterator itr = selection.begin();
             itr != selection.end();
             itr++) {
            (*itr)->GetAccumulatedTransformations(&p, &q);
            q.Normalize();
            p = q.GetInverse().RotateVector(d);
            Quaternion<float> rot(ratio*PI*0.1,p);
            rot.Normalize();
            Quaternion<float> rot1 = (*itr)->GetRotation();
            rot1 *= rot;
            rot1.Normalize();
            (*itr)->SetRotation(rot1);
        }
        return true;
    }
    return false;
}

bool TransformationTool::RotationStrategy::GrabWidget(int x, 
                                                      int y, 
                                                      ISceneSelection& select,
                                                      set<TransformationNode*> selection,
                                                      Viewport& vp) {
    int offset = 2;
    Vector<3,float> p;
    Quaternion<float> q;
    (*selection.begin())->GetAccumulatedTransformations(&p, &q);
    axisgrabbed = axiswidget.GrabAxis(x, y, offset, select, *selection.begin(), vp, dir, true);
    if (!axisgrabbed) return false;
    axisproj = select.Project(p + q.RotateVector(dir), vp) 
        - select.Project(p, vp); 
    if (axisproj.IsZero()) 
        //@todo: what todo with this value???
        axisproj = Vector<2,float>(0.0,-PI*0.01);
    axisproj = Vector<2,float>(-axisproj[1], axisproj[0]);
    p = dir;
    ratio = (p.GetLength())/(axisproj.GetLength());
    mouse = Vector<2,float>(x, y);
    axisproj.Normalize();
    return true;
}

bool TransformationTool::RotationStrategy::GrabSelection(int x, 
                                                         int y, 
                                                         ISceneSelection& select, 
                                                         TransformationNode* node,
                                                         set<TransformationNode*> selection,
                                                         Viewport& vp) {
    return false;
}

void TransformationTool::RotationStrategy::RenderWidget(IViewingVolume& vv, ISceneNode* context) {
    axiswidget.Render(vv, context, true);
}

bool TransformationTool::RotationStrategy::Reset() {
    if (axisgrabbed) {
        axisgrabbed = false;
        return true;
    }
    return false;
}

TransformationTool::ScalingStrategy::ScalingStrategy(): 
    axisgrabbed(false), t(NULL)
{
}

TransformationTool::ScalingStrategy::~ScalingStrategy() {
} 


bool TransformationTool::ScalingStrategy::Transform(int x, 
                                                     int y,
                                                     int dx,
                                                     int dy,
                                                     ISceneSelection& select,
                                                     set<TransformationNode*> selection,
                                                     Viewport& vp) {
    if (axisgrabbed) {
        Vector<2,float> dmouse = Vector<2,float>(x, y) - mouse;
        mouse = Vector<2,float>(x, y);
        Vector<3,float> d = dir.GetNormalize() * (axisproj * dmouse) * ratio;
        for (set<TransformationNode*>::iterator itr = selection.begin();
             itr != selection.end();
             itr++) {
            (*itr)->SetScale((*itr)->GetScale()+d);
        }
        return true;
    }
    if (t) {
            float scl = 1.0;
            if (dy < 0) 
                scl = 1.05;
            if (dy > 0)
                scl = 0.95;
        for (set<TransformationNode*>::iterator itr = selection.begin();
             itr != selection.end();
             itr++) {
            (*itr)->SetScale((*itr)->GetScale()*scl);
        }
        return true;
    }
    return false;
}

bool TransformationTool::ScalingStrategy::GrabWidget(int x, 
                                                      int y, 
                                                      ISceneSelection& select,
                                                      set<TransformationNode*> selection,
                                                      Viewport& vp) {
    int offset = 2;
    Vector<3,float> p;
    Quaternion<float> q;
    (*selection.begin())->GetAccumulatedTransformations(&p, &q);
    axisgrabbed = axiswidget.GrabAxis(x, y, offset, select, *selection.begin(), vp, dir, true);
    if (!axisgrabbed) return false;
    axisproj = select.Project(p + q.RotateVector(dir), vp) 
        - select.Project(p, vp); 
    if (axisproj.IsZero()) 
        //@todo: what todo with this value???
        axisproj = Vector<2,float>(0.0,3.0);
    ratio = (dir.GetLength())/(axisproj.GetLength())*0.1;
    mouse = Vector<2,float>(x, y);
    axisproj.Normalize();
    return true;
}

bool TransformationTool::ScalingStrategy::GrabSelection(int x, 
                                                        int y, 
                                                        ISceneSelection& select, 
                                                        TransformationNode* node,
                                                        set<TransformationNode*> selection,
                                                        Viewport& vp) {
    // check for selection hit
    set<TransformationNode*>::iterator i = selection.find(node);
    if (i != selection.end()) {
        t = node;
        return true;
    }
    return false;
}

void TransformationTool::ScalingStrategy::RenderWidget(IViewingVolume& vv, ISceneNode* context) {
    axiswidget.Render(vv, context, true);
}

bool TransformationTool::ScalingStrategy::Reset() {
    if (axisgrabbed) {
        axisgrabbed = false;
        return true;
    }
    if (t) {
        t = NULL;
        return true;
    }
    return false;
}


} // NS Utils
} // NS OpenEngine

