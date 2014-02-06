#include <iostream>
#include <string>
#include "curl/curl.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "Fetcher.h"


CFetcher::CFetcher(std::string srv, std::string prx)
{
	server = srv;
	proxy = prx;
	curlHandler = curl_easy_init();
	curl_easy_setopt(curlHandler, CURLOPT_PROXY, proxy.c_str());
	resourcesFolder = "content/transport";
}

CFetcher::~CFetcher()
{
	curl_easy_cleanup(curlHandler);
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

rapidjson::Document CFetcher::GetJSONContent(std::string url)
{
	std::string readBuffer;

	curl_easy_setopt(curlHandler, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curlHandler, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curlHandler, CURLOPT_WRITEDATA, &readBuffer);	
	curl_easy_perform(curlHandler);

	rapidjson::Document jsonDocument;
    jsonDocument.Parse<0>(readBuffer.c_str());
	return jsonDocument;
}

std::string CFetcher::GetImageContent(std::string url)
{
	std::string filename = resourcesFolder + "/" + url.substr(url.find_last_of("/") + 1) + ".png";

	FILE* file = fopen(filename.c_str(), "w+");
	
	curl_easy_setopt(curlHandler, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curlHandler, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt(curlHandler, CURLOPT_WRITEDATA, file);
	curl_easy_perform(curlHandler);
	fclose(file);
	return filename;
}
