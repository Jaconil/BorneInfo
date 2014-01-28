//coding: utf-8
#ifndef _TransportFetcher_h
	#define _TransportFetcher_h

#include <vector>
#include "../Fetcher.h"
#include "Line.h"
#include "Schedule.h"
#include "Traffic.h"

class CTransportFetcher : CFetcher
{
public:
	std::string linesUrl;
	std::string nearUrl;
	std::string logoUrl;
	std::string mapButtonUrl;
	std::string trafficUrl;

	std::vector<CLine> lines;
	std::map<std::string, std::string> near;
	std::map<EDirection, std::pair<std::string, std::string> > next;
	std::vector<CTraffic> traffic;
	
	CTransportFetcher(std::string, std::string);
	virtual ~CTransportFetcher();

	std::vector<CLine> FetchLines();
	std::map<std::string, std::string> FetchNearLines();
	CSchedule FetchSchedule(std::string, std::string, EDirection);
	std::vector<CTraffic> FetchTraffic(std::string);
	std::map<EDirection, std::pair<std::string, std::string> > FetchNext(std::string, std::string);

	void FetchLogo();
	void FetchMapButton();

private: 
	bool HandleError(rapidjson::Document&);
};

#endif
