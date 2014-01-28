#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "Line.h"

CLine::CLine(
	std::string id,
	std::string name,
        std::string direction1,
        std::string direction2,
        std::string map,
        std::string tileset,
        int top,
        int left,
	std::map<std::string, std::string> stops,
        std::map<std::string, std::vector< std::string> > connected
	)
{
	this->id = id;
	this->name = name;
	this->direction1 = direction1;
        this->direction2 = direction2;
        this->map = map;
        this->tileset = tileset;
        this->top = top;
        this->left = left;
	this->stops = stops;
        this->connected = connected;
}

CLine::~CLine() { }

std::string CLine::toString()
{
        std::ostringstream out;
        out << "id: " << id << "\n";
        out << "name: " << name << "\n";
        out << "direction1: " << direction1 << "\n";
        out << "direction2: " << direction2 << "\n";
        out << "map: " << map << "\n";
        out << "tileset: " << tileset << "\n";
        out << "top: " << top << "\n";
        out << "left: " << left << "\n\n";
        out << "stops: " << "\n";

        for (std::map<std::string, std::string>::iterator it = stops.begin(); it != stops.end(); ++it)
        {
            out << "\t" << "id: " << it->first << "\n";
            out << "\t" << "id: " << it->second << "\n";
        }

        out << "\nconnected: " << "\n";

        for (std::map<std::string, std::vector< std::string> >::iterator it = connected.begin(); it != connected.end(); ++it)
        {
            out << "\t" << "line: " << it->first << " ";

            std::stringstream connectedStops;
            std::copy(it->second.begin(), it->second.end(), std::ostream_iterator<std::string>(connectedStops, " "));

            out << "\t" << "stops: " << connectedStops.str() << "\n";
        }

        return out.str();
}
