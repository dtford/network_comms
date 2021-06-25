#include "comms_args.h"
#include <iostream>

CommsArgsReader::CommsArgsReader(int argc, char **argv)
{
	// default
	command = ERROR;

	//for testing only
	if( argc >= 2)
	{
		for(int arg_index = 1; arg_index < argc; ++arg_index)
		{
			std::string arg = argv[arg_index];
			if(arg == "-u" || arg == "--url")
			{
				// read url
				if( url.empty() && ( ++arg_index < argc ))
				{
					url = argv[arg_index];
				}
				else
				{
					command = ERROR;
					break;
				}	
			}
			else if(arg == "-o" || arg == "--post")
			{
				command = POST;
			}
			else if(arg == "-g" || arg == "--get")
			{
				command = GET;
			}
			else if(arg == "-p" || arg == "--put")
			{
				command = PUT;
			}
			else if(arg == "-d" || arg == "--delete")
			{
				command = DELETE;
			}
			else if(arg == "-h" || arg == "--help")
			{
				command = HELP;
			}
			else
			{
				if( !url.empty() && command != ERROR )
				{
					// read the rest as the freetext string
					freeText = arg;
					for( ++arg_index; arg_index < argc; ++arg_index)
					{
						freeText.append( " " );
						freeText.append( argv[arg_index] );
					}
				}
				else
				{
					command = ERROR;
					break;
				}
			}
		}
	}
	else
	{
		command = ERROR;
	}
	if( ( ( command == PUT ) || ( command == POST ) ) && freeText.empty() )
	{
		// post and put require a string, so this is an error
		command = ERROR;
	}
	if( ( command != ERROR ) && ( command != HELP ) && ( url.empty() ) )
	{
		// request commands require a url, so this is an error
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
