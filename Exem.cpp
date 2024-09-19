#include <iostream>
#include <string>
#include "Command_manager.h"
#include "Log.h"

int main()
{
	Log* log = Log::create();
	command_manager::Command command;
	std::cout << "Program has started\n(If you need command list - print 'help')\nPlease, enter command:\n";
	do {
		try {
			std::string str;
			std::getline(std::cin, str);
			command >> str;
		}
		catch (std::runtime_error& ex) {
			std::cout << ex.what() << std::endl;
		}
		std::cout << "------------------------------------\n";
	} while (!command.end());
	std::cout << "Program has ended\n"
	<< "------------------------------------\n";
	Log::release();
}
