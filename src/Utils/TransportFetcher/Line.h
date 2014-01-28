//coding: utf-8
#ifndef _Line_h
	#define _Line_h

#include <string>
#include <map>
#include <vector>

typedef enum {
	DIRECTION1,
	DIRECTION2
} EDirection;


class CLine
{
public:
	std::string id;
	std::string name;
	std::string direction1;
	std::string direction2;
	std::string map;
	std::string tileset;
	int top, left;
	
	std::map<std::string, std::string> stops;
        std::map<std::string, std::vector< std::string> > connected;
	
	CLine(std::string,
        std::string,
        std::string,
        std::string,
        std::string,
        std::string,
        int,
        int,
        std::map<std::string, std::string>,
        std::map<std::string, std::vector< std::string> >);
    
	virtual ~CLine();
    
        std::string toString();
};

#endif
