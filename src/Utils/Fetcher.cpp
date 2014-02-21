#include <iostream>
#include <string>
#include <sys/stat.h>
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
	resourcesFolder = "../content/Transport";
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
	std::string readBuffer;
	std::string filename = resourcesFolder + "/" + url.substr(url.find_last_of("/") + 1) + ".png";

	struct stat buffer;
	bool fileExists = stat(filename.c_str(), &buffer) == 0;
	bool downloadFile = false;
	time_t mTime = buffer.st_mtime;
	time_t currentTime = time(NULL);
	int cache = 1;
	int maxElapsedSeconds = 600;

	if(fileExists)
	{
		if(currentTime - mTime > maxElapsedSeconds)
		{
			downloadFile = true;

			curl_easy_setopt(this->curlHandler, CURLOPT_URL, url.c_str());
			curl_easy_setopt(this->curlHandler, CURLOPT_NOBODY, 1);
			curl_easy_setopt(this->curlHandler, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(this->curlHandler, CURLOPT_WRITEHEADER, &readBuffer);
			curl_easy_perform(this->curlHandler);

			int start = readBuffer.find("Cache-Control:");

			if(start != -1)
			{
				int end = readBuffer.substr(start).find("\n");
				sscanf(readBuffer.substr(start, end).c_str(),"Cache-Control: max-age=%d, private", &cache);
			}
		}
	}

	if((cache <= 0 && downloadFile) || ! fileExists)
	{
		FILE* file = fopen(filename.c_str(), "w+");
		
		curl_easy_reset(this->curlHandler);
		curl_easy_setopt(this->curlHandler, CURLOPT_NOBODY, 0);
		curl_easy_setopt(this->curlHandler, CURLOPT_URL, url.c_str());
		curl_easy_setopt(this->curlHandler, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(this->curlHandler, CURLOPT_WRITEDATA, file);
		curl_easy_perform(this->curlHandler);
		
		fclose(file);
	}

	return filename;
}
