//coding: utf-8
#ifndef _TransportScrollable_h
	#define _TransportScrollable_h

#include "../../RenderingObject.h"
#include <Utility/3D/CTriangle.h>
#include <Utility/3D/CQuad.h>
#include "../ScreenTransport.h"

class CScreenTransport;
class CScrollBar;

class IScrollable
{
public:    
    virtual float GetBoxWidth() = 0;
    virtual float GetBoxHeight() = 0;
    virtual float GetContentHeight() = 0;
    virtual OpenUtility::CMat4x4<float> GetMVPMatrix() = 0;
    virtual CScreenTransport* GetScreen() = 0;
    
    inline CScrollBar* GetScrollBar() {return this->ScrollBar;}
    
protected:
    CScrollBar* ScrollBar;
};

class CScrollBar : public IRenderingObject
{
public:
    CScrollBar(IRenderingObjectComm *obj, IScrollable* scrollableContent);
    CScrollBar(const CScrollBar &obj) : IRenderingObject(obj) {};
    CScrollBar& operator=(const CScrollBar &obj) {IRenderingObject::operator=(obj); return(*this);};
    ~CScrollBar() {};
    
    void Scroll(float value);
    void StopScroll();
    void ScrollTo(float scroll);
    
    void Init();
	void UnInit();
	bool PreRender();
	void Render();
    
    inline void SetScrollHeight(float scrollHeight) {this->ScrollHeight = ScrollHeight;}
    inline float GetScrollHeight() {return this->ScrollHeight;}
    
    inline float GetCurrentScroll() {return this->CurrentScroll;}
    
private:
    IScrollable* ScrollableContent;
    
    float Width, Height;
    
    float ScrollHeight; // 1 = pas de scroll, sinon ]0, 1[
    float CurrentScroll; // 0 = en haut, 1 = en bas
    float ScrollValue; // Vitesse de scroll courante
    
    float TotalBarHeight;
    
    OpenUtility::CTriangle* UpArrow;
    OpenUtility::CTriangle* DownArrow;
    OpenUtility::CQuad* Bar;
    
    OpenUtility::CMat4x4<float> MVPmatrix;
    OpenUtility::CMat4x4<float> BarMatrix;
    
    bool MustRender;
};

#endif

