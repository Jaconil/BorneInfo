//coding: utf-8
#ifndef _TransportScreenSchedule_h
	#define _TransportScreenSchedule_h

#include "../ScreenTransport.h"

class CTScreenSchedule : public CSubScreenTransport
{
public:
    CTScreenSchedule(IRenderingObjectComm *obj, CScreenTransport* CommonScreen) : CSubScreenTransport(obj, CommonScreen) {};
    CTScreenSchedule(const CTScreenSchedule &obj) : CSubScreenTransport(obj) {};
    CTScreenSchedule& operator=(const CTScreenSchedule &obj) {CSubScreenTransport::operator=(obj); return(*this);};
    ~CTScreenSchedule() {};
	    
    void Init();
	void UnInit();
	bool PreRender();
	void Render();
    
    std::string GetTitle() {return "Horaires ligne X";};

private:
    
};

#endif
