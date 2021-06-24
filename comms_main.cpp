#include <iostream>

#include <curl/curl.h>

#include "comms_args.h"

int main(int argc, char **argv)
{
	CommsArgsReader argsReader(argc, argv);
	
	// for testing only
	if(argsReader.getCommand() != CommsArgsReader::ERROR )
	{
		std::cout << argsReader.getURL() << "\n";
	}
	else
	{
		std::cout << "ERROR\n";
	}
}
