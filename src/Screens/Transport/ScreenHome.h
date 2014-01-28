//coding: utf-8
#ifndef _TransportScreenHome_h
	#define _TransportScreenHome_h

#include "../ScreenTransport.h"

class CTScreenHome : public CSubScreenTransport
{
public:
    CTScreenHome(IRenderingObjectComm *obj, CScreenTransport* CommonScreen) : CSubScreenTransport(obj, CommonScreen) {};
    CTScreenHome(const CTScreenHome &obj) : CSubScreenTransport(obj) {};
    CTScreenHome& operator=(const CTScreenHome &obj) {CSubScreenTransport::operator=(obj); return(*this);};
    ~CTScreenHome() {};
	    
    void Init();
	void UnInit();
	bool PreRender();
	void Render();
    
    std::string GetTitle() {return "Prochains bus";};

private:
    
};

#endif

