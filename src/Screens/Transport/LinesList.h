//coding: utf-8
#ifndef _TransportLinesList_h
	#define _TransportLinesList_h

#include <vector>
#include <string>
#include "../../RenderingObject.h"
#include "../ScreenTransport.h"
#include "ScrollBar.h"

class CScreenTransport;

class CLinesList : public IRenderingObject, IScrollable
{
public:
    CLinesList(IRenderingObjectComm *obj, CScreenTransport* commonScreen);
    CLinesList(const CLinesList &obj) : IRenderingObject(obj) {};
    CLinesList& operator=(const CLinesList &obj) {IRenderingObject::operator=(obj); return(*this);};
    ~CLinesList();
	    
    void Init();
	void UnInit();
	bool PreRender();
	void Render();
    
    void Update();
    
    float GetBoxWidth() {return this->Width;}
    float GetBoxHeight() {return this->Height;}
    float GetContentHeight() {return this->ContentHeight;}
    OpenUtility::CMat4x4<float> GetMVPMatrix() {return this->MVPmatrix;}
    CScreenTransport* GetScreen() {return this->CommonScreen;}
    
private:
    CScreenTransport* CommonScreen;
    
    float Width, Height;
    float LeftCrop, TopCrop, RightCrop, BottomCrop;
    
    float ContentHeight;
    
    EScreenTransport CurrentScreen;
    
    std::vector<std::string> StopConnected;
    std::vector<std::string> LineConnected;
    std::vector<std::string> OtherLines;
    
    OpenUtility::CQuad* Background;
    
    CScrollBar* ScrollBar;
    
    OpenUtility::C3DText* StopConnectedTitle;
    OpenUtility::C3DText* LineConnectedTitle;
    OpenUtility::C3DText* NearLinesTitle;
    OpenUtility::C3DText* OtherLinesTitle;
    OpenUtility::C3DText* AllLinesTitle;
    
    OpenUtility::CMat4x4<float> MVPmatrix;
    OpenUtility::CMat4x4<float> ListMatrix;
};

#endif
