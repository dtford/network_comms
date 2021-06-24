#include "comms_args.h"

CommsArgsReader::CommsArgsReader(int argc, char **argv)
{
	//for testing only
	if( argc >= 2)
	{
		url = argv[1];
	}
	else
	{
		command = ERROR;
	}	
}

std::string CommsArgsReader::getURL()
{
	return url;
}

CommsArgsReader::CommandType CommsArgsReader::getCommand()
{
	return command;
}

std::string CommsArgsReader::getFreeText()
{
	return freeText;
}
