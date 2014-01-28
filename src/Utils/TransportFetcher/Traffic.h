//coding: utf-8
#ifndef _Traffic_h
	#define _Traffic_h

#include <string>

class CTraffic
{
public:
	CTraffic(std::string, std::string, std::string, std::string);
	virtual ~CTraffic();

	std::string toString();

private:
	std::string title;
	std::string start;
	std::string end;
	std::string summary;
};

#endif
