#include <iostream>

#include <curl/curl.h>

#include "comms_args.h"

int main(int argc, char **argv)
{
	CommsArgsReader argsReader(argc, argv);
	
	switch(argsReader.getCommand() )
	{
		case CommsArgsReader::POST:
		{
			break;
		}
		case CommsArgsReader::GET:
		{
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
}
