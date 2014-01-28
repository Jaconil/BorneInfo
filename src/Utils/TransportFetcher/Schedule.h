//coding: utf-8
#ifndef _Schedule_h
	#define _Schedule_h

#include <map>
#include <vector>
#include <string>

class CSchedule
{
public:
	std::map<int, std::vector<int> > schedule;
	CSchedule(std::map<int, std::vector<int> >);
	virtual ~CSchedule();
	std::string toString();
};

#endif
