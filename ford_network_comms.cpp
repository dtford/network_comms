/* To compile:
 * $(CXX) -o hw ford_network_comms.cpp -lcurl -uClibc
 * e.g.:
 *   "~/buildroot/output/host/usr/bin/arm-linux-g++ --sysroot=~/buildroot/output/staging  -o test *.cpp  -lcurl -uClibc -lc"
 *  "~/buildroot/output/host/usr/bin/arm-linux-g++ --sysroot=/home/<username>/buildroot/output/staging -o hw coford_network_comms.cpp -lcurl -uClibc"
 *
 * Note that this is C++ code and requires that Buildroot is C++ enabled.
 * However, I used the C curl library (libcurl) rather than the C++ curl library (curlpp)
 * to minimize the need for special support.
 *
 * This code was written by Dallen Ford
 * for the Network Communications assignment of ECE 531 at UNM
 *
 */

#include <algorithm>
#include <iostream>
#include <string>

#include <cstring>

#include <curl/curl.h>

#define OK			0
#define INIT_ERR	1
#define REQ_ERR		2

class CommsArgsReader
{
	public:

		enum CommandType
		{
			POST,
			GET,
			PUT,
			DELETE,
			HELP,
			ERROR
		};

		CommsArgsReader(int argc, char **argv);
		
		std::string getURL();

		CommandType getCommand();

		std::string getFreeText();

	private:
		std::string url;
		CommandType command;
		std::string freeText;
};

CommsArgsReader::CommsArgsReader(int argc, char **argv)
{
	// default
	command = ERROR;

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
					freeText.append(arg);
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

size_t read_callback(char *ptr, size_t size, size_t nmemb, void *text)
{
	size_t retcode = snprintf( ptr, nmemb, "%s", static_cast<char *>( text ) );
	return std::min( retcode, nmemb );
}

struct Memory
{
	char *response;
	size_t size;

	Memory()
	{
		response = 0;
		size = 0;
	}	
};

size_t write_callback(char *data, size_t size, size_t nmemb, void *userdata)
{
	size_t realsize = size * nmemb;
	char *ptr;
	Memory *mem = static_cast<Memory *>( userdata );
	
	if(mem->response) 
	{
		ptr = static_cast<char *>( realloc(mem->response, mem->size + realsize + 1) );
	}
	else
	{
		ptr = static_cast<char *>( malloc(realsize + 1) );
	}
	if(ptr == NULL)
		return 0;

	mem->response = ptr;
	memcpy( &( mem->response[mem->size] ), data, realsize );
	mem->size += realsize;
	mem->response[mem->size] = 0;

	return realsize;
}

int main( int argc, char **argv )
{
	CommsArgsReader argsReader( argc, argv );
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if(!curl)
	{
		return INIT_ERR;
	}

	Memory buffer;

	if( curl && !argsReader.getURL().empty() )
	{
		curl_easy_setopt(curl, CURLOPT_URL, argsReader.getURL().c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	}

	long response_code;

	switch(argsReader.getCommand() )
	{
		case CommsArgsReader::POST:
		{
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);
			curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, argsReader.getFreeText().c_str());
			res = curl_easy_perform(curl);
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			std::cout << response_code << std::endl << buffer.response << std::endl;
			break;
		}
		case CommsArgsReader::GET:
		{
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
			res = curl_easy_perform(curl);
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			std::cout << response_code << std::endl << buffer.response << std::endl;
			break;
		}
		case CommsArgsReader::PUT:
		{
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
			
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
			curl_easy_setopt(curl, CURLOPT_READDATA, argsReader.getFreeText().c_str());
			curl_easy_setopt(curl, CURLOPT_INFILESIZE, (curl_off_t)argsReader.getFreeText().size());
			res = curl_easy_perform(curl);
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			std::cout << response_code << std::endl << buffer.response << std::endl;

			break;
		}
		case CommsArgsReader::DELETE:
		{
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
			if( !argsReader.getFreeText().empty() )
			{
				curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, argsReader.getFreeText().c_str());
			}
			res = curl_easy_perform(curl);
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
			std::cout << response_code << std::endl << buffer.response << std::endl;

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
			std::cout << "  TEXT\t\t\tfree text field to be included in the request.\n";
		}
	}
	if(curl) 
	{
		curl_easy_cleanup(curl);
	}
	return OK;
}
