//coding: utf-8
#ifndef _ScreenEvent_h
	#define _ScreenEvent_h

#include "Screen.h"

class CScreenEvent : public IScreen
{
public:
	CScreenEvent(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image=NULL);
	CScreenEvent(const CScreenEvent &obj);
	virtual ~CScreenEvent();
	CScreenEvent& operator=(const CScreenEvent &obj);
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
    
    void OnKeyDown(unsigned int id,int keyCode) {};
	void OnKeyUp(unsigned int id,int keyCode) {};
	void OnMouseMove(unsigned int id,double x,double y) {};
};

#endif
