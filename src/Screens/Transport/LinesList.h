//coding: utf-8
#ifndef _TransportLinesList_h
	#define _TransportLinesList_h

#include <vector>
#include <string>
#include "../../RenderingObject.h"
#include "../ScreenTransport.h"

class CScreenTransport;

class CLinesList : public IRenderingObject
{
public:
    CLinesList(IRenderingObjectComm *obj, CScreenTransport* commonScreen);
    CLinesList(const CLinesList &obj) : IRenderingObject(obj) {};
    CLinesList& operator=(const CLinesList &obj) {IRenderingObject::operator=(obj); return(*this);};
    ~CLinesList() {};
	    
    void Init();
	void UnInit();
	bool PreRender();
	void Render();

private:
    CScreenTransport* CommonScreen;
    
    float Left, Right, Top, Bottom;
    
    std::vector<std::string> StopConnected;
    std::vector<std::string> LineConnected;
    std::vector<std::string> OtherLines;
    
    // Gestion de la scrollbar
    float ScrollHeight; // 1 = pas de scroll, sinon ]0, 1[
    float CurrentScroll; // 0 = en haut, 1 = en bas
    
    OpenUtility::CQuad* Background;
};

#endif
