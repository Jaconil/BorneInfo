#include <string>
#include "Traffic.h"

CTraffic::CTraffic(std::string title, std::string start, std::string end, std::string summary)
{
	this->title = title;
	this->start = start;
	this->end = end;
	this->summary = summary;
}

CTraffic::~CTraffic() { }

std::string CTraffic::toString()
{
	return "summary: " + this->summary + " start: " + this->start + " end: " + this->end + " summary: " + this->summary;
}
