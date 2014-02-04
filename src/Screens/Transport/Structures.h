//coding: utf-8
#ifndef _TransportStructures_h
	#define _TransportStructures_h

enum EScreenTransport
{
    ST_HOME = 0,
    ST_SCHEDULE,
    ST_STOPS,
    ST_TRAFFIC,
    ST_LINEMAP,
    ST_MAP,
};

enum EFontTransport
{
    FT_TITLE,
    FT_MENUTITLE,
    FT_MENU
};

struct STransportStatus
{
    EScreenTransport PreviousScreen;
    EScreenTransport CurrentScreen;
    std::string CurrentLine;
    std::string CurrentStop;
//	EDirection CurrentDirection;
//	CSchedule CurrentSchedule;
//	std::map<Traffic> CurrentTraffic;

    STransportStatus()
    {
        PreviousScreen = ST_HOME;
        CurrentScreen = ST_HOME;
        CurrentLine = "";
        CurrentStop = "";
    }
};    

#endif