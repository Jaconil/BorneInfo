#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"

#include "Line.h"
#include "TransportFetcher.h"


CTransportFetcher::CTransportFetcher(std::string srv, std::string prx) : CFetcher(srv, prx)
{
	this->linesUrl = server + "/borne/web/bus/lines";
	this->nearUrl = server + "/borne/web/bus/near";
	this->mapButtonUrl = server + "/borne/web/bus/mapButton";
	this->logoUrl = server + "/borne/web/bus/logo";
}

CTransportFetcher::~CTransportFetcher() { }

bool CTransportFetcher::HandleError(rapidjson::Document& jsonDocument)
{
	return jsonDocument.IsObject() && jsonDocument.HasMember("error_num");
}

std::vector<CLine> CTransportFetcher::FetchLines()
{
	rapidjson::Document jsonDocument = CFetcher::GetJSONContent(this->linesUrl);

	if(! HandleError(jsonDocument))
	{
		this->lines.clear();
	
		for (rapidjson::SizeType i = 0; i < jsonDocument.Size(); i++)
		{
			rapidjson::Value& jsonLine = jsonDocument[i];
			
			std::string id = jsonLine["id"].GetString();
			std::string name = jsonLine["name"].GetString();
			std::string direction1 = jsonLine["sens1"].GetString();
			std::string direction2 = jsonLine["sens2"].GetString();
			std::string tileset = "";
			int top = 0;
			int left = 0;
			int width = 0;
			int height = 0;
			std::map<std::string, std::string> stops;
			std::map<std::string, std::vector< std::string> > connected;
			
			const rapidjson::Value& rootLogo = jsonLine["logo"];

			if( ! rootLogo.IsNull())
			{
				top = rootLogo["top"].GetInt();
				left = rootLogo["left"].GetInt();
				width = rootLogo["width"].GetInt();
				height = rootLogo["height"].GetInt();
				std::string tilesetUrl = server + rootLogo["tileset"].GetString();
				tileset = GetImageContent(tilesetUrl);
			}

			const rapidjson::Value& root = jsonLine["stops"];
			
			for (rapidjson::Value::ConstValueIterator itr = root.Begin(); itr != root.End(); ++itr)
				stops.insert(std::pair<std::string, std::string>((*itr)["id"].GetString(), (*itr)["name"].GetString()));
				
			const rapidjson::Value& rootConnected = jsonLine["connected"];
			
			for (rapidjson::Value::ConstValueIterator itr = rootConnected.Begin(); itr != rootConnected.End(); ++itr)
			{
				std::string connectedLine;

				if((*itr)["line"].IsInt())
				{
					std::ostringstream out;
					out << (*itr)["line"].GetInt();
					connectedLine = out.str();
				}
				else
				{
					connectedLine = (*itr)["line"].GetString();
				}
				
				const rapidjson::Value& rootConnectedStops = (*itr)["stops"];
				std::vector<std::string> connectedStops;
				
				for (rapidjson::SizeType i = 0; i < rootConnectedStops.Size(); i++)
					connectedStops.push_back(rootConnectedStops[i].GetString());
				
				connected.insert(std::pair<std::string, std::vector<std::string> >(connectedLine, connectedStops));
			}

			std::string map = "";			

			CLine line( id,
						name,
						direction1,
						direction2,
						map,
						tileset,
						top,
						left,
						width,
						height,
						stops,
						connected);

			this->lines.push_back(line);
		}
	}

	return this->lines;
}


std::map<std::string, std::string> CTransportFetcher::FetchNearLines()
{	
	rapidjson::Document jsonDocument = CFetcher::GetJSONContent(this->nearUrl);

	if(! HandleError(jsonDocument))
	{
		this->near.clear();

		for (rapidjson::SizeType i = 0; i < jsonDocument.Size(); i++)
		{
			rapidjson::Value& jsonLine = jsonDocument[i];
			this->near[jsonLine["line"].GetString()] = jsonLine["stop"].GetString();
		}
	}

	return this->near;
}

CSchedule CTransportFetcher::FetchSchedule(std::string lineId, std::string stop, EDirection direction)
{
	std::map<int, std::vector<int> > schedule;

	std::string url = server + "/borne/web/bus/lines/" + lineId + "/schedule/" + stop + "/";
	url += (direction == DIRECTION1) ? "1" : "2";
	std::cout << url << std::endl;
	rapidjson::Document jsonDocument = CFetcher::GetJSONContent(url);

	if(! HandleError(jsonDocument))
	{
		for (rapidjson::Value::ConstMemberIterator itrHours = jsonDocument.MemberBegin(); itrHours != jsonDocument.MemberEnd(); ++itrHours)
		{
			int hour;
			std::vector<int> minutes;
			std::istringstream(itrHours->name.GetString()) >> hour;

			const rapidjson::Value& minutesJson = itrHours->value;
			
			for (rapidjson::Value::ConstValueIterator itrMinutes = minutesJson.Begin(); itrMinutes != minutesJson.End(); ++itrMinutes)
			{
				int minute;
				std::istringstream(itrMinutes->GetString()) >> minute;
				minutes.push_back(minute);
			}
			schedule[hour] = minutes;
		}
	}

	return CSchedule(schedule);
}

std::map<EDirection, std::pair<std::string, std::string> > CTransportFetcher::FetchNext(std::string line, std::string stop)
{
	std::string url = server + "/borne/web/bus/lines/" + line + "/next/" + stop;
	std::cout << "url: " << url << std::endl;
	rapidjson::Document jsonDocument = CFetcher::GetJSONContent(url);

	bool result = HandleError(jsonDocument);

	if(! result)
	{
		this->next.clear();
		for (rapidjson::SizeType i = 0; i < jsonDocument.Size(); i++)
		{
			rapidjson::Value& jsonLine = jsonDocument[i];
			EDirection direction = (jsonLine["sens"].GetInt() == 1) ? DIRECTION1 : DIRECTION2;
			std::string time = jsonLine["time"].GetString();
			std::string next = jsonLine["next"].GetString();
			this->next[direction] = std::pair<std::string, std::string>(time, next);
		}
	}

	return this->next;
}

std::vector<CTraffic> CTransportFetcher::FetchTraffic(std::string line)
{
	std::string trafficUrl = server + "/borne/web/bus/lines/" + line + "/traffic";

	rapidjson::Document jsonDocument = CFetcher::GetJSONContent(trafficUrl);

	if(! HandleError(jsonDocument))
	{
		this->traffic.clear();
		for (rapidjson::SizeType i = 0; i < jsonDocument.Size(); i++)
		{
			rapidjson::Value& jsonLine = jsonDocument[i];
			std::string title = jsonLine["title"].GetString();
			std::string start = jsonLine["start"].GetString();
			std::string end = jsonLine["end"].GetString();
			std::string summary = jsonLine["summary"].GetString();
			this->traffic.push_back(CTraffic(title, start, end, summary));
		}
	}

	return this->traffic;
}

void CTransportFetcher::FetchLogo()
{
	rapidjson::Document jsonDocument = CFetcher::GetJSONContent(this->logoUrl);

	if(! HandleError(jsonDocument))
	{
		this->logoInfo.top = jsonDocument["top"].GetInt();
		this->logoInfo.left = jsonDocument["left"].GetInt();
		this->logoInfo.width = jsonDocument["width"].GetInt();
		this->logoInfo.height = jsonDocument["height"].GetInt();
		std::string tilesetUrl = server + jsonDocument["tileset"].GetString();
		this->logoInfo.file = GetImageContent(tilesetUrl);
	}
}

void CTransportFetcher::FetchMapButton()
{
	this->mapButtonInfo.file = GetImageContent(this->mapButtonUrl);
}
