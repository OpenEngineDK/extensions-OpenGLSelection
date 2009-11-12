// On screen display - Widget interface, a widget has position and dimension.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_WIDGETS_INTERFACE_WIDGET_
#define _OE_WIDGETS_INTERFACE_WIDGET_

#include <Math/Vector.h>
#include <Resources/IFontResource.h>

namespace OpenEngine {
namespace Widgets {

using Resources::IFontResourcePtr;
class IWidgetRenderer;
class WidgetRenderer;
class PointingDevice;

/**
 * On Screen Display Widget Interface.
 * 
 * A widget has coordinates (x,y) and dimensions (width, height).
 * 
 * It can receive focus (e.g from a cursor) and it can be
 * activated/deactivated.
 *  
 * Widget rendering is handled by the IRenderer class. Rendering
 * uses visitor pattern and as such all widgets must implements the
 * accept method with a callback to the IRenderer.
 *
 * An example use pattern with a mouse could be: 
 *
 *  * OnMouseMoveEvent(x,y) { w.FocusAt(x,y); }
 * 
 *  * OnMouseButtonDownEvent() { w.ActivateFocus() }
 *
 *  * OnMouseButtonUpEvent() { w.Reset() }
 *
 *  * OnRenderPhase() { r.Render(w) } 
 *
 * @see IRenderer
 *
 * @class IWidget IWidget.h OpenGLSelection/Utils/IWidget.h
 */
class IWidget {
public:
    /**
     * Empty destructor.
     */
    virtual ~IWidget() {};
    
    /**
     * Get the position of the widget.
     * @return the x and y coordinates of the widget.
     */
    virtual Math::Vector<2,int> GetPosition() = 0;
    /**
     * Get the dimensions of the widget.
     * @return the width and height of the widget.
     */
    virtual Math::Vector<2,int> GetDimensions() = 0;
    /**
     * Set the position of the widget.
     * @param pos the x and y coordinates of the widget.
     */
    virtual void SetPosition(Math::Vector<2,int> pos) = 0;
    /**
     * Set the dimensions of the widget.
     * @param dim the width and height of the widget.
     */
    virtual void SetDimensions(Math::Vector<2,int> dim) = 0;
 
    /**
     * The rendering (visitor callback) method.  Every widget should
     * implement this method with a call to the corresponding Render()
     * method of the IRenderer.
     *
     * @param r the on screen display renderer.
     */
    virtual void Accept(IWidgetRenderer& r) = 0;

    // input methods
    /**
     * Check if the coordinates (x,y) is inside a widget. 
     *
     * If the widget is a compound widget then it must call its
     * sub-widgets recursively and return the first sub-widget that
     * produces a hit.
     *
     * @param x the first hit coordinate.
     * @param y the second hit coordinate.
     * @return the widget that was hit or NULL if no widget was hit.
     */
    virtual IWidget* WidgetAt(int x, int y) = 0;   

    /**
     * Same as WidgetAt with the addition that a non-hit triggers a
     * call to SetFocus(false) and a hit triggers a call to
     * SetFocus(true).
     *
     * @see WidgetAt
     * @param x the first hit coordinate.
     * @param y the second hit coordinate.
     * @return the widget that was hit or NULL if no widget was hit.
     */
    virtual IWidget* FocusAt(int x, int y) = 0;    

    /**
     * Same as WidgetAt with the addition that a non-hit triggers a
     * call to SetActive(false) and a hit triggers a call to
     * SetActive(true).
     *
     * @see WidgetAt
     * @param x the first hit coordinate.
     * @param y the second hit coordinate.
     * @return the widget that was hit or NULL if no widget was hit.
     */
    virtual IWidget* ActivateAt(int x, int y) = 0; 

    /**
     * Activate a focused widget
     * 
     * If the widget is a compound widget then it must make a call to
     * its sub-widgets.
     *
     * @return the widget that was activated.
     */
    virtual IWidget* ActivateFocus() = 0;         

    /**
     * Reset the widget.
     * 
     * This usually means SetActive(false) but functionality may vary.
     *
     */ 
   virtual void Reset() = 0;   
    
    //state change methods
    /**
     * Change the active state of the widget.
     * 
     * @param active the active state.
     */
    virtual void SetActive(bool active) = 0;

    /**
     * Get the active state of the widget.
     * 
     * @return the active state.
     */
    virtual bool GetActive() = 0;

    /**
     * Change the focus state of the widget.
     * 
     * @param focus the focus state.
     */
    virtual void SetFocus(bool focus) = 0;

    /**
     * Get the focus state of the widget.
     * 
     * @return the focus state.
     */
    virtual bool GetFocus() = 0;

    // used to create font textures from fonts determined by the renderer.
    virtual void SetupFonts(WidgetRenderer& r) = 0;

    
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_WIDGETS_INTERFACE_WIDGET_
