//coding: utf-8
#ifndef _TransportLinesList_h
	#define _TransportLinesList_h

#include <vector>
#include <string>
#include <Utility/3D/CTextureMultiQuad.h>
#include "../../RenderingObject.h"
#include "../ScreenTransport.h"
#include "ScrollBar.h"

class CScreenTransport;

class CLinesList : public IRenderingObject, public IScrollable
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
    
    void Update(bool screenChanged);
    
    float GetBoxWidth() {return this->Width;}
    float GetBoxHeight() {return this->Height;}
    float GetContentHeight() {return this->ContentHeight;}
    OpenUtility::CMat4x4<float> GetMVPMatrix() {return this->MVPmatrix;}
    CScreenTransport* GetScreen() {return this->CommonScreen;}
    
private:
    void RenderTitle(std::string text);
    void RenderLine(int texId, std::string direction1, std::string direction2);
    
private:
    CScreenTransport* CommonScreen;
    
    float Width, Height;
    float LeftCrop, TopCrop, RightCrop, BottomCrop;
    
    float ContentHeight;
    float CurrentHeight;
    
    EScreenTransport CurrentScreen;
    
    std::vector<std::string> StopConnected;
    std::vector<std::string> LineConnected;
    std::vector<std::string> OtherLines;
    
    OpenUtility::CQuad* Separator;
    OpenUtility::CTextureMultiQuad* LineLogo;

    OpenUtility::C3DText* LinesTitle;
    OpenUtility::C3DText* LinesDirection;
    
    OpenUtility::CMat4x4<float> MVPmatrix;
    OpenUtility::CMat4x4<float> ScrollMatrix;
    OpenUtility::CMat4x4<float> ListMatrix;
};

#endif
