//coding: utf-8
#ifndef _ScreenOrganigram_h
	#define _ScreenOrganigram_h

#include "Screen.h"

class CScreenOrganigram : public IScreen
{
public:
	CScreenOrganigram(IRenderingObjectComm *obj,OpenUtility::ITextureQuad *image=NULL);
	CScreenOrganigram(const CScreenOrganigram &obj);
	virtual ~CScreenOrganigram();
	CScreenOrganigram& operator=(const CScreenOrganigram &obj);
	void Init();
	void UnInit();
	bool PreRender();
	void Render();
    
    void OnKeyDown(unsigned int id,int keyCode) {};
	void OnKeyUp(unsigned int id,int keyCode) {};
	void OnMouseMove(unsigned int id,double x,double y) {};

private:
	OpenUtility::CQuad *Background;
};

#endif
