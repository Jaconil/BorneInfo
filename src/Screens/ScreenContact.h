//coding: utf-8
#ifndef _ScreenContact_h
	#define _ScreenContact_h

#include "Screen.h"

class CScreenContact : public IScreen
{
public:
	CScreenContact(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image=NULL);
	CScreenContact(const CScreenContact &obj);
	virtual ~CScreenContact();
	CScreenContact& operator=(const CScreenContact &obj);
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
    
    void OnKeyDown(unsigned int id,int keyCode) {};
	void OnKeyUp(unsigned int id,int keyCode) {};
	void OnMouseMove(unsigned int id,double x,double y) {};
};

#endif
