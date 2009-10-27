// On screen display - circular slider.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_UTILS_OSD_CIRCULAR_SLIDER_
#define _OE_UTILS_OSD_CIRCULAR_SLIDER_

#include <Utils/OSDIWidget.h>
#include <Resources/IFontTextureResource.h>

#include <Math/Vector.h>

namespace OpenEngine {
namespace Utils {

class OSDRenderer;

/**
 * On Screen Display Circular Slider Class.  
 *
 * A circular slider changes value in a predefined interval when
 * manipulated by the input device.
 *
 * @class OSDCircularSlider OSDCircularSlider.h OpenGLSelection/Utils/OSDCircularSlider.h
 */
class OSDCircularSlider: public OSDIWidget {
private:
    Resources::IFontTextureResourcePtr texr;
    int x, y, width, height;
    bool active, focus;
    float value, startAngle, sweep, step;
    Math::Vector<2,float> center, focusVec;
    int AngleToQuadrant(float angle);
public:
    OSDCircularSlider(OSDRenderer& r);
    virtual ~OSDCircularSlider();

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
    float GetValue();
    void SetValue(float value);
    float GetStartAngle();
    float GetSweep();
    Resources::ITextureResourcePtr GetTexture();
};

} // NS Utils
} // NS OpenEngine
#endif //_OE_UTILS_OSD_CIRCULAR_SLIDER_
