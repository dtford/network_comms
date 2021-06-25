#include <iostream>

#include <curl/curl.h>

#include "comms_args.h"

#define OK			0
#define INIT_ERR	1
#define REQ_ERR		2

int main(int argc, char **argv)
{
	CommsArgsReader argsReader(argc, argv);
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if(!curl)
	{
		return INIT_ERR;
	}

	if(curl && !argsReader.getURL().empty())
	{
		curl_easy_setopt(curl, CURLOPT_URL, argsReader.getURL().c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	}

	switch(argsReader.getCommand() )
	{
		case CommsArgsReader::POST:
		{
			break;
		}
		case CommsArgsReader::GET:
		{
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
			res = curl_easy_perform(curl);
			long response_code;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			std::cout << "Get result code: " << response_code << std::endl;
			if( res != CURLE_OK) 
			{
				std::cerr << "CURLcode: " << res << std::endl;
				return REQ_ERR;
			}
			else
			{
			}
			break;
		}
		case CommsArgsReader::PUT:
		{
			break;
		}
		case CommsArgsReader::DELETE:
		{
			break;
		}
		case CommsArgsReader::ERROR:
		{
			std::cerr << "Input error.\n";
		}
		case CommsArgsReader::HELP:
		{
			std::cout << "Usage: " << argv[0] << " [command] [-u|--url URL [TEXT]]\n";
			std::cout << "Commands/fields:\n";
			std::cout << "  -d, --delete\t\tSend delete request. Requires URL.\n";
			std::cout << "  -g, --get\t\tSend get request. Requires URL.\n";
			std::cout << "  -h, --help\t\tShow this help message.\n";
			std::cout << "  -o, --post\t\tSend post request. Requires URL and TEXT.\n";
			std::cout << "  -p, --put\t\tSend put request. Requires URL and TEXT.\n";
			std::cout << "  -u URL, --url URL\tSpecify URL for request.\n";
			std::cout << "  TEXT\t\t\t free text field to be included in the request.\n";
		}
	}
	if(curl) 
	{
		curl_easy_cleanup(curl);
	}
	return OK;
}
