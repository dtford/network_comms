#ifndef COMMS_ARGS_H
#define COMMS_ARGS_H

#include <string>

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

#endif
