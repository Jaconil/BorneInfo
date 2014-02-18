//coding: utf-8
#ifndef _ScreenMap_h
	#define _ScreenMap_h

#include "Screen.h"

class CScreenMap : public IScreen
{
public:
	CScreenMap(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image=NULL);
	CScreenMap(const CScreenMap &obj);
	virtual ~CScreenMap();
	CScreenMap& operator=(const CScreenMap &obj);
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
    
    void OnKeyDown(unsigned int id,int keyCode) {};
	void OnKeyUp(unsigned int id,int keyCode) {};
	void OnMouseMove(unsigned int id,double x,double y) {};
};

#endif
