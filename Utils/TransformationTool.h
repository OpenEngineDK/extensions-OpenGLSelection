// ITool - Transformation Tool
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_TRANSFORMATION_TOOL_
#define _OE_UTILS_TRANSFORMATION_TOOL_

#include <Meta/OpenGL.h>
#include <Utils/ITool.h>
#include <Utils/PointingDevice.h>
#include <Utils/SelectionSet.h>
#include <Core/IListener.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>
#include <Geometry/Plane.h>
#include <Scene/RenderNode.h>
#include <Resources/ITextureResource.h>
#include <Scene/SearchTool.h>
#include <set>


namespace OpenEngine {
    namespace Renderers {
        class IRenderer;
        class IRenderingView;
    }
    namespace Scene {
        class TransformationNode;
        class SceneNode;
        class SearchTool;
    }
    namespace Display {
        class IViewingVolume;
    }
namespace Utils {
class ISceneSelection;

/**
 * Transformation Tool
 * Used to manipulate selected geometry using TransformationNode's.
 * @class TransformationTool TransformationTool.h OpenGLSelection/Utils/TransformationTool.h
 */
class TransformationTool : 
        public ITool, 
        public Core::IListener<Utils::SelectionSet<Scene::ISceneNode>::ChangedEventArg> {
private:
    class AxisWidget {
    private:
        class Axis : public Scene::RenderNode {
        public:
            GLUquadricObj* quadr;
            float rot;
            Math::Vector<3,float> rotaxis;
            Math::Vector<3,float> dir;
            Math::Vector<4,float> colr;
            Axis(float rot, 
                 Math::Vector<3,float> rotaxis, 
                 Math::Vector<3,float> dir, 
                 Math::Vector<4,float> colr);
            virtual ~Axis();
            void Apply(Renderers::IRenderingView* rv);
        };
        Scene::SceneNode* axes;
        Axis *xaxis, *yaxis, *zaxis;
        class XAxis : public Axis {
        public: 
            XAxis();
        };
        class YAxis : public Axis {
        public: 
            YAxis();
        };
        class ZAxis : public Axis {
        public: 
            ZAxis();
        };
    public:        
        AxisWidget();
        virtual ~AxisWidget();
        void Render(Display::IViewingVolume& vv, Scene::ISceneNode* context, bool rotate);
        bool GrabAxis(int x, int y, int offset, ISceneSelection& select, Scene::ISceneNode* context, Display::Viewport& vp, Math::Vector<3,float>& dir, bool rotate);
    };

    class ITransformationStrategy {
    public: 
        virtual ~ITransformationStrategy() {};
        virtual bool Transform(int x, 
                               int y, 
                               int dx,
                               int dy,
                               ISceneSelection& select, 
                               std::set<Scene::TransformationNode*> selection, 
                               Display::Viewport& vp) = 0;
        virtual bool GrabWidget(int x, 
                                int y, 
                                ISceneSelection& select, 
                                std::set<Scene::TransformationNode*> selection, 
                                Display::Viewport& vp) = 0;
        virtual bool GrabSelection(int x, 
                                   int y, 
                                   ISceneSelection& select,
                                   Scene::TransformationNode* node, 
                                   set<Scene::TransformationNode*> selection, 
                                   Display::Viewport& vp) = 0;
        virtual void RenderWidget(Display::IViewingVolume& vv, 
                                  Scene::ISceneNode* context) = 0;
        virtual bool Reset() = 0;
    };

    class TranslationStrategy: public ITransformationStrategy {
    private:
        //axis movement
        AxisWidget axiswidget;
        bool axisgrabbed;
        Math::Vector<3,float> dir;
        Math::Vector<2,float> axisproj;
        float ratio;
        Math::Vector<2,float> mouse;
        // camera plane movement
        Geometry::Plane plane;
        Scene::TransformationNode* t;
        Math::Vector<3,float> oldp;
        void UpdateTransformationNodes(float x, float y,
                                       Display::Viewport& vp,
                                       ISceneSelection& ssel,
                                       std::set<Scene::TransformationNode*> selection);
        void CalcMovePlane(float x, float y, 
                           Math::Vector<3,float> startPos, 
                           Display::Viewport& vp,
                           ISceneSelection& ssel);
        Scene::SearchTool search;
    public:
        TranslationStrategy();
        virtual ~TranslationStrategy();
        bool Transform(int x, 
                       int y, 
                       int dx,
                       int dy,
                       ISceneSelection& select, 
                       std::set<Scene::TransformationNode*> selection, 
                       Display::Viewport& vp);
        bool GrabWidget(int x, 
                        int y, 
                        ISceneSelection& select, 
                        std::set<Scene::TransformationNode*> selection, 
                        Display::Viewport& vp);
        bool GrabSelection(int x, 
                           int y, 
                           ISceneSelection& select, 
                           Scene::TransformationNode* node, 
                           set<Scene::TransformationNode*> selection, 
                           Display::Viewport& vp);
        void RenderWidget(Display::IViewingVolume& vv, Scene::ISceneNode* context);        
        bool Reset();
    };

    class RotationStrategy: public ITransformationStrategy {
    private:
        //axis movement
        AxisWidget axiswidget;
        bool axisgrabbed;
        Math::Vector<3,float> dir;
        Math::Vector<2,float> axisproj;
        float ratio;
        Math::Vector<2,float> mouse;
        Scene::SearchTool search;
    public:
        RotationStrategy();
        virtual ~RotationStrategy();
        bool Transform(int x, 
                       int y,
                       int dx,
                       int dy,
                       ISceneSelection& select, 
                       std::set<Scene::TransformationNode*> selection, 
                       Display::Viewport& vp);
        bool GrabWidget(int x, 
                        int y, 
                        ISceneSelection& select, 
                        std::set<Scene::TransformationNode*> selection, 
                        Display::Viewport& vp);
        bool GrabSelection(int x, 
                           int y, 
                           ISceneSelection& select, 
                           Scene::TransformationNode* node, 
                           set<Scene::TransformationNode*> selection, 
                           Display::Viewport& vp);
        void RenderWidget(Display::IViewingVolume& vv, 
                          Scene::ISceneNode* context);        
        bool Reset();
    };

    class ScalingStrategy: public ITransformationStrategy {
    private:
        //axis movement
        AxisWidget axiswidget;
        bool axisgrabbed;
        Math::Vector<3,float> dir;
        Math::Vector<2,float> axisproj;
        float ratio;
        Math::Vector<2,float> mouse;
        Scene::SearchTool search;
        Scene::TransformationNode* t;
    public:
        ScalingStrategy();
        virtual ~ScalingStrategy();
        bool Transform(int x, 
                       int y,
                       int dx,
                       int dy,
                       ISceneSelection& select, 
                       std::set<Scene::TransformationNode*> selection, 
                       Display::Viewport& vp);
        bool GrabWidget(int x, 
                        int y, 
                        ISceneSelection& select, 
                        std::set<Scene::TransformationNode*> selection, 
                        Display::Viewport& vp);
        bool GrabSelection(int x, 
                           int y, 
                           ISceneSelection& select, 
                           Scene::TransformationNode* node, 
                           set<Scene::TransformationNode*> selection, 
                           Display::Viewport& vp);
        void RenderWidget(Display::IViewingVolume& vv, 
                          Scene::ISceneNode* context);        
        bool Reset();
    };

    // @todo: maybe this should be in SelectionTool
    class SelectionRenderer : public Scene::ISceneNodeVisitor {
        void VisitGeometryNode(Scene::GeometryNode* node);
        void VisitTransformationNode(Scene::TransformationNode* node);
    };

    class Button: public Scene::RenderNode {
    private:
    public:
        Resources::ITextureResourcePtr fg, bg;
        Math::Vector<4,float> fgc, bgc;
        float posx, posy, size;
        bool selected, focus;
        Button(float posx, float posy, float size);
        void Apply(Renderers::IRenderingView* rv);
    };
    SelectionRenderer sr;    
    std::set<Scene::TransformationNode*> selection;
    TranslationStrategy translation;
    RotationStrategy rotation;
    ScalingStrategy scaling;
    ITransformationStrategy* transformation;
    // buttons
    Scene::SceneNode* buttons;
    Button *traBtn, *rotBtn, *sclBtn;
    Scene::SearchTool search;
    inline void ApplyTransformationNode(Scene::TransformationNode* node);
public:
    TransformationTool();
    ~TransformationTool();
    bool Handle(PointingDevice::MovedEventArg arg);
    bool Handle(PointingDevice::PressedEventArg arg);
    bool Handle(PointingDevice::ReleasedEventArg arg);
    void Handle(SelectionSet<Scene::ISceneNode>::ChangedEventArg arg);
    void Render(Display::IViewingVolume& vv, Renderers::IRenderer& r);
    void RenderOrtho(Display::IViewingVolume& vv, Renderers::IRenderer& r);
};

} // NS Utils
} // NS OpenEngine
#endif 
