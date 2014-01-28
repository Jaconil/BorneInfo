#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>
#include "Schedule.h"

CSchedule::CSchedule(std::map<int, std::vector<int> > schedule) { this-> schedule = schedule; }
CSchedule::~CSchedule() {}

std::string CSchedule::toString()
{
	std::ostringstream out;
	for (std::map<int, std::vector<int> >::iterator it = schedule.begin(); it != schedule.end(); ++it)
    {
    	out << "hour: " << it->first << "minutes: [";
        std::copy(it->second.begin(), it->second.end()-1, std::ostream_iterator<int>(out, ","));
        out << it->second.back() << "]" << std::endl;
    }
    return out.str();
}
