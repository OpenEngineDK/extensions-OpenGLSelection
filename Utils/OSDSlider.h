// On screen display - slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_OSD_SLIDER_
#define _OE_UTILS_OSD_SLIDER_

#include <Utils/OSDIWidget.h>

#include <Math/Vector.h>
#include <Resources/ITextureResource.h>

namespace OpenEngine {
namespace Utils {
/**
 * On Screen Display Slider Class.  
 *
 * A slider changes value in the interval [0.0,1.0] when manipulated
 * by the input device.
 *
 * @class OSDButton OSDButton.h OpenGLSelection/Utils/OSDButton.h
 */
class OSDSlider: public OSDIWidget {
private:
    Math::Vector<4,float> colr;
    int x, y, width, height;
    bool active, focus;
    float value;
public:
    OSDSlider();
    virtual ~OSDSlider();
    
    Math::Vector<2,int> GetPosition();
    Math::Vector<2,int> GetDimensions();
    void SetPosition(Math::Vector<2,int> pos);
    void SetDimensions(Math::Vector<2,int> dim);
    void Accept(OSDIRenderer& r);
    bool GetActive();
    void SetActive(bool active);
    bool GetFocus();
    void SetFocus(bool focus);
    OSDIWidget* WidgetAt(int x, int y);
    OSDIWidget* FocusAt(int x, int y);
    OSDIWidget* ActivateAt(int x, int y);
    OSDIWidget* ActivateFocus();
    void Reset();

    Math::Vector<4,float> GetColor();
    void SetColor(Math::Vector<4,float> colr);

    float GetValue();
    void SetValue(float value);
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_OSD_SLIDER_
