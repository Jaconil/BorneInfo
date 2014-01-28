//coding: utf-8
#ifndef _Fetcher_h
        #define _Fetcher_h

#include <string>
#include "curl/curl.h"
#include "rapidjson/document.h"

class CFetcher
{
public:
	CFetcher(std::string, std::string);
	virtual ~CFetcher();
	
	rapidjson::Document GetJSONContent(std::string);
	void GetImageContent(std::string);

protected:
	std::string server;
	std::string proxy;
	CURL* curlHandler;
	std::string resourcesFolder;
};

#endif
