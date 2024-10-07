#include "Command_manager.h"

void command_manager::Command::cleaner(std::string& str) {
	while (str.find(" ") != std::string::npos) {
		auto it = str.begin();
		while (it != str.end()) {
			if (*it == ' ') {
				str.erase(it);
				break;
			}
			it++;
		}
	}
}

void command_manager::Command::help_func(Command* obj) {
	obj->set_function_name("help_func");
	std::cout << "\nCommand list:\n"
		<< cyan"help " << white "- print command list\n"
		<< cyan"guide " << white "- guide\n\n"

		<< yellow "find bus (keys) " << white "- find bus which match inputted keys(keys must be written like(key1, key2, ..., key))\n"
		<< yellow"find client (keys) " << white "- find client which match inputted keys(keys must be written like(key1, key2, ..., key))\n"
		<< yellow"find seats (keys) " << white "- find all tickets\n"
		<< yellow"calculate (bus id, start city, arrival city, have benefit) " << white "- calculate cost (have benefit must be 0 or 1)\n\n"

		<< green "add client (name_surname, bus id, have benefit, date) " << white "- register new client\n"
		<< "\t(have benefit must be 0 or 1, date must be day.month)\n"
		<< high_red"delete client (name) " << white "- remove client by it`s name\n\n"

		<< high_magenta"sort client (key) " << white "- set sorting key for 'show clients' command\n"
		<< "\t(key can be -- reset, name, ticket, bus id, date)\n"
		<< high_magenta"sort bus (key) " << white "- set sorting key for 'show buses' command\n"
		<< "\t(key can be -- reset, bus id, departure, arrival, cost, max seats, max benefit, booked seats, booked benefit)\n"
		<< high_magenta"sort seats (key) " << white "- set sorting key for 'show seats' command\n"
		<< "\t(key can be -- reset, bus id, date, tickets)\n\n"

		<< magenta "show all buses " << white "- shows every bus\n"
		<< magenta"show all clients " << white "- show every client\n"
		<< magenta"show all seats " << white "- show every seat\n"
		<< magenta"show all " << white "- show everything\n\n"

		<< cyan "clear " << white "- clear console\n"
		<< cyan"admin (password) " << white "- become admin\n"
		<< cyan"exit " << white "- end program\n";

	if (obj->admin) {
		std::cout << high_yellow"\n------------------------------------\n";
		std::cout << white"\nAdmin commands:\n"
			<< green"admin add bus (departure, arrival, max seats, benefit seats, cost, route) " << white "- create and upload new bus\n"
			<< "\tarrival and departure must be written like hours:minutes\n"
			<< "\troute must be written like City1-City2-....-End City\n"
			<< high_red"admin delete bus (bus id) " << white "- delete bus in database by it`s id\n\n"

			<< yellow"admin update " << white "- update every file\n"
			<< yellow"admin change password (new password) " << white "- rewrite password\n\n"
			<< cyan"admin resize bus database (new limit) " << white "- rewrite limit of bus database\n"
			<< high_red"\tDo not use, if you don't know what you do\n"
			<< cyan"admin resize client database (new limit) " << white "- rewrite limit of client database\n"
			<< high_red"\tDo not use, if you don't know what you do\n"
			<< cyan"admin resize seats database (new limit) " << white "- rewrite limit of seats database\n"
			<< high_red"\tDo not use, if you don't know what you do\n"
			<< cyan"leave admin " << white "- leave admin rights\n";
	}
	obj->log->save(obj, 0);
}
void command_manager::Command::exit_func(Command* obj) {
	obj->set_function_name("exit_func");
	obj->exit = 1;

	obj->log->save(obj, 0);
}
void command_manager::Command::clear_func(Command* obj) {
	obj->set_function_name("clear_func");
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif 

	obj->log->save(obj, 0);
}
void command_manager::Command::guide_func(Command* obj) {
	obj->set_function_name("guide_func");
	std::cout << "\nCommand list queue for user:\n"
		<< magenta"If you need to start up the project use 'admin add bus (properties)' command with admin rights\n"
		<< yellow"1. use 'find bus (keys)' command" << white" to find bus with keys(for example: (key1, ...., keyN))\n"
		<< yellow"2. use 'calculate (bus id, start city, arrival city, have benefit (0 for 'not' and 1 for 'yes'))' command" << white" to calculate cost\n"
		<< yellow"3. use 'add client (name_surname, bus id, have benefit (0 for 'not' and 1 for 'yes'), date)'" << white" to add client in databases\n"
		<< yellow"4. use 'delete client (name_surname)'" << white" to delete user, if needed\n"
		<< green"\nAny other command is optional\n"
		<< "See 'help' for more information\n";

	obj->log->save(obj, 0);
}

void command_manager::Command::admin_update_func(Command* obj) {
	obj->set_function_name("admin_update_func");
	if (!obj->admin) {
		obj->log->save(obj, 6);
		throw std::runtime_error("\nYou must have admin rights to execute this command");
	}
	obj->bus_base.update();
	obj->client_base.update();
	obj->seats_base.update();
	obj->log->save(obj, 0);
}
void command_manager::Command::admin_func(std::string& str, Command* obj) {
	obj->set_function_name("admin_func");
	if (obj->admin) {
		obj->log->save("error (admin_func): user already have admin rights");
		throw std::runtime_error("\nYou are already an admin");
	}
	if (str == obj->password) {
		obj->admin = 1;
		std::cout << "\nWelcome back, admin!\n";
		obj->log->save("success (admin_func): user become an admin");
	}
	else {
		std::cout << "\nInvalid password\n";
		obj->log->save("fail (admin_func): invalid password");
	}
	obj->log->save(obj, 0);
}
void command_manager::Command::admin_add_bus_func(std::string& str, Command* obj) {
	obj->set_function_name("admin_add_bus_func");
	if (!obj->admin) {
		obj->log->save(obj, 6);
		throw std::runtime_error("\nYou must have admin rights to execute this command");
	}
	Bus bus;

	cleaner(str);

	std::size_t size = str.find(":");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nInvalid departure syntax\nSee 'help' for more information");
	}
	std::string buff_str = str.substr(0, size);
	if (buff_str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	str = str.substr(size + 1, std::string::npos);

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nInvalid departure syntax\nSee 'help' for more information");
	}
	std::string buff_min = str.substr(0, size);
	if (buff_min.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	if (std::stoi(buff_str, 0) >= 24 || std::stoi(buff_min, 0) >= 60) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nInvalid departure value\nSee 'help' for more information");
	}

	bus.set_departure(buff_str, buff_min);
	str = str.substr(size + 1, std::string::npos);
	//
	size = str.find(":");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nInvalid arrival syntax\nSee 'help' for more information");
	}
	buff_str = str.substr(0, size);
	if (buff_str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}
	str = str.substr(size + 1, std::string::npos);

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nInvalid arrival syntax\nSee 'help' for more information");
	}
	buff_min = str.substr(0, size);
	if (buff_min.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	if (std::stoi(buff_str, 0) >= 24 || std::stoi(buff_min, 0) >= 60) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nInvalid arrival value\nSee 'help' for more information");
	}

	bus.set_arrival(buff_str, buff_min);
	str = str.substr(size + 1, std::string::npos);

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	if (str.substr(0, size).empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}
	bus.set_max_seats(std::stoi(str, &size));
	str = str.substr(size + 1, std::string::npos);

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	if (str.substr(0, size).empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	unsigned int checker = std::stoi(str, &size);
	if (checker > bus.get_max_seats()) {
		obj->log->save("error (admin_add_bus_func): not enough seats");
		throw std::runtime_error("\nAmount of benefit seats must be less or equal to max of seats");
	}
	bus.set_max_benefit_seats(std::stoi(str, &size));
	str = str.substr(size + 1, std::string::npos);
	if (str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	if (str.substr(0, size).empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}
	bus.set_cost(std::stoi(str, &size));
	str = str.substr(size + 1, std::string::npos);

	while (size != std::string::npos) {
		size = str.find("-");
		buff_str = str.substr(0, size);
		bus.set_route(buff_str);
		str = str.substr(size + 1, std::string::npos);
	}

	bus.generate(6);
	obj->bus_base.put(obj->bus_base.size() + 1, bus);
	obj->bus_base.update();
	obj->log->save(obj, 0);
}
void command_manager::Command::admin_delete_bus_func(std::string& str, Command* obj) {
	obj->set_function_name("admin_delete_bus_func");
	if (!obj->admin) {
		obj->log->save(obj, 6);
		throw std::runtime_error("\nYou must have admin rights to execute this command");
	}
	std::size_t buff_size = str.find_first_not_of("0123456789");
	if (buff_size != std::string::npos) {
		obj->log->save("error (admin_delete_bus_func): invalid bus id");
		throw std::runtime_error("\nBus id must consist of integers\nSee 'help' for more information");
	}
	buff_size = str.length();
	unsigned int buff = std::stoi(str, &buff_size);
	auto result = obj->bus_base.find(buff);
	if (result.empty()) {
		obj->log->save(obj, 4);
		throw std::runtime_error("\nThere isn't any bus with such id");
	}
	auto find_result = obj->seats_base.find(buff);
	if (find_result.empty())
		obj->bus_base.erase(result.at(0));
	else {
		if (obj->seats_base.get(find_result.at(0)).get_size() == 0) {
			obj->bus_base.erase(result.at(0));
			obj->seats_base.erase(find_result.at(0));
		}
		else {
			obj->log->save("error (admin_delete_bus_func): clients are in database");
			throw std::runtime_error("\nThe clients are still in database");
		}
	}
	obj->log->save(obj, 0);
}
void command_manager::Command::leave_admin_func(Command* obj) {
	obj->set_function_name("leave_admin_func");
	if (!obj->admin) {
		obj->log->save(obj, 6);
		throw std::runtime_error("\nYou must have admin rights to execute this command");
	}
	obj->admin = 0;
	clear_func(obj);
	obj->log->save(obj, 0);
}
void command_manager::Command::admin_change_password_func(std::string& str, Command* obj) {
	obj->set_function_name("admin_change_password");
	if (!obj->admin) {
		obj->log->save(obj, 6);
		throw std::runtime_error("\nYou must have admin rights to execute this command");
	}
	obj->password = str;
	obj->admin_config.get_ptr(0)->set_password(str);
	obj->admin_config.update();
	obj->log->save(obj, 0);
}

void command_manager::Command::admin_resize_bus_database_func(std::string& str, Command* obj) {
	obj->set_function_name("admin_resize_bus_database_func");
	if (!obj->admin) {
		obj->log->save(obj, 6);
		throw std::runtime_error("\nYou must have admin rights to execute this command");
	}

	std::size_t size = str.find_first_not_of("1234567890");
	if (size != std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid input");
	}

	obj->admin_config.get_ptr(0)->set_bus(std::stoull(str, 0));
	obj->admin_config.update();
	obj->bus_base.resize(obj->admin_config.get(0).get_bus());

	obj->log->save(obj, 0);
}
void command_manager::Command::admin_resize_client_database_func(std::string& str, Command* obj) {
	obj->set_function_name("admin_resize_client_database_func");
	if (!obj->admin) {
		obj->log->save(obj, 6);
		throw std::runtime_error("\nYou must have admin rights to execute this command");
	}

	std::size_t size = str.find_first_not_of("1234567890");
	if (size != std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid input");
	}

	obj->admin_config.get_ptr(0)->set_client(std::stoull(str, 0));
	obj->admin_config.update();
	obj->client_base.resize(obj->admin_config.get(0).get_client());

	obj->log->save(obj, 0);
}
void command_manager::Command::admin_resize_seats_database_func(std::string& str, Command* obj) {
	obj->set_function_name("admin_resize_seats_database_func");
	if (!obj->admin) {
		obj->log->save(obj, 6);
		throw std::runtime_error("\nYou must have admin rights to execute this command");
	}

	std::size_t size = str.find_first_not_of("1234567890");
	if (size != std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid input");
	}

	obj->admin_config.get_ptr(0)->set_seats(std::stoull(str, 0));
	obj->admin_config.update();
	obj->seats_base.resize(obj->admin_config.get(0).get_seats());

	obj->log->save(obj, 0);
}

void command_manager::Command::show_all_buses_func(Command* obj) {
	obj->set_function_name("show_all_buses_func");
	if (obj->bus_base.size() == 0) {
		obj->log->save(obj, 4);
		throw std::runtime_error("\nThere isn't any bus in database");
	}

	std::cout << "\n";
	if (!obj->sort_bus.empty()) {
		std::cout << "Sorting key: " << obj->sort_bus << "\n\n";
	}

	for (unsigned int i = 0; i < obj->bus_base.get_capacity(); i++) {
		Bus buff = obj->bus_base.get(i);
		std::cout << buff;
	}
	obj->log->save(obj, 0);
}
void command_manager::Command::show_all_clients_func(Command* obj) {
	obj->set_function_name("show_all_clients_func");

	if (obj->client_base.size() == 0) {
		obj->log->save(obj, 3);
		throw std::runtime_error("\nThere isn't any client in database");
	}

	std::cout << "\n";
	if (!obj->sort_client.empty()) {
		std::cout << "Sorting key: " << obj->sort_client << "\n\n";
	}

	unsigned int pos = 0;

	while (pos != obj->client_base.get_capacity()) {
		Client client_buff = obj->client_base.get(pos);
		std::cout << client_buff;
		pos++;
	}

	obj->log->save(obj, 0);
}
void command_manager::Command::show_all_seats_func(Command* obj) {
	obj->set_function_name("show_all_seats_func");
	if (obj->seats_base.size() == 0) {
		obj->log->save(obj, 5);
		throw std::runtime_error("\nThere isn't any seat in database");
	}

	unsigned int pos = 0;
	std::cout << "\n";
	if (!obj->sort_seats.empty()) {
		std::cout << "Sorting key: " << obj->sort_seats << "\n\n";
	}

	while (pos != obj->seats_base.get_capacity()) {
		Seats seat_buff = obj->seats_base.get(pos);
		std::cout << seat_buff;
		pos++;
	}
	obj->log->save(obj, 0);
}
void command_manager::Command::show_all_func(Command* obj) {
	obj->set_function_name("show_all_func");
	show_all_buses_func(obj);
	std::cout << white"------------------------------------\n";
	show_all_clients_func(obj);
	std::cout << white"------------------------------------\n";
	show_all_seats_func(obj);
	obj->log->save(obj, 0);
}

void command_manager::Command::find_bus_func(std::string& str, Command* obj) {
	obj->set_function_name("find_bus_func");
	cleaner(str);
	std::cout << "\n";

	std::vector<pos_type> result;
	std::size_t size = str.find(",");
	if (size == std::string::npos) {
		size = str.find_first_not_of("0123456789");
		if (size != std::string::npos) {
			size = str.find(":");
			if (size == std::string::npos) {
				result = obj->bus_base.find(str);
				for (int i = 0; i < result.size(); i++) {
					Bus buff = obj->bus_base.get(result.at(i));
					std::cout << buff;
				}
			}
			else {
				std::pair<std::string, std::string> buff_pair;
				buff_pair.first = str.substr(0, size);
				str = str.substr(size + 1, std::string::npos);
				buff_pair.second = str;

				result = obj->bus_base.find(buff_pair);
				for (int i = 0; i < result.size(); i++) {
					Bus buff = obj->bus_base.get(result.at(i));
					std::cout << buff;
				}
			}
		}
		else {
			unsigned int buff_int = std::stoi(str, &size);
			result = obj->bus_base.find(buff_int);
			for (int i = 0; i < result.size(); i++) {
				Bus buff = obj->bus_base.get(result.at(i));
				std::cout << buff;
			}
		}
	}
	else {
		std::string buff_str = str.substr(0, size);
		if (buff_str.empty()) {
			obj->log->save(obj, 2);
			throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
		}

		size = buff_str.find_first_not_of("0123456789");
		if (size != std::string::npos) {
			size = buff_str.find(":");
			if (size == std::string::npos) {
				result = obj->bus_base.find(buff_str);
			}
			else {
				std::pair<std::string, std::string> buff_pair;
				buff_pair.first = buff_str.substr(0, size);
				buff_str = buff_str.substr(size + 1, std::string::npos);
				buff_pair.second = buff_str;

				result = obj->bus_base.find(buff_pair);
			}
		}
		else {
			unsigned int buff_int = std::stoi(str, &size);
			result = obj->bus_base.find(buff_int);
		}
		size = str.find(",");
		str = str.substr(size + 1, std::string::npos);

		while (size != std::string::npos) {
			size = str.find(",");
			buff_str = str.substr(0, size);
			if (buff_str.empty()) {
				obj->log->save(obj, 2);
				throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
			}
			std::size_t buff_size = buff_str.find_first_not_of("0123456789");
			if (buff_size != std::string::npos) {
				buff_size = buff_str.find(":");
				if (buff_size == std::string::npos) {
					result = obj->bus_base.find(result, buff_str);
				}
				else {
					std::pair<std::string, std::string> buff_pair;
					buff_pair.first = buff_str.substr(0, buff_size);
					buff_str = buff_str.substr(buff_size + 1, std::string::npos);
					buff_pair.second = buff_str;

					result = obj->bus_base.find(result, buff_pair);
				}
			}
			else {
				unsigned int buff_int = std::stoi(str, &buff_size);
				result = obj->bus_base.find(result, buff_int);
			}
			str = str.substr(size + 1, std::string::npos);
		}

		for (int i = 0; i < result.size(); i++) {
			Bus buff = obj->bus_base.get(result.at(i));
			std::cout << buff;
		}
	}
	if (result.empty()) {
		obj->log->save(obj, 4);
		throw std::runtime_error("\nNone bus was found");
	}
	obj->log->save(obj, 0);
}
void command_manager::Command::find_client_func(std::string& str, Command* obj) {
	obj->set_function_name("find_client_func");
	std::cout << std::endl;
	cleaner(str);

	bool executed = 0;
	std::size_t size = str.find(",");
	if (size == std::string::npos) {
		std::vector<pos_type> result;
		size = str.find_first_not_of("0123456789");

		if (size != std::string::npos) {
			std::pair<std::string, std::string> buff_pair;
			buff_pair.first = str.substr(0, size);
			buff_pair.second = str.substr(size + 1, std::string::npos);
			result = obj->client_base.find(buff_pair);
		}
		else {
			size = str.find_first_not_of("01");

			if (size != std::string::npos) {
				unsigned int buff_int = std::stoi(str, &size);
				result = obj->client_base.find(buff_int);
			}
			else {
				bool buff_bool = std::stoi(str, &size);
				result = obj->client_base.find(buff_bool);
			}
		}
		if (result.empty()) {
			obj->log->save(obj, 3);
			throw std::runtime_error("\nNone client was found");
		}
		for (int i = 0; i < result.size(); i++) {
			Client buff = obj->client_base.get(result.at(i));
			std::cout << buff;
		}

	}
	else {
		std::string buff_str = str.substr(0, size);
		if (buff_str.empty()) {
			obj->log->save(obj, 2);
			throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
		}

		std::vector<pos_type> result;
		size = buff_str.find_first_not_of("0123456789");
		if (size != std::string::npos) {
			result = obj->client_base.find(buff_str);
		}
		else {
			size = buff_str.find_first_not_of("01");
			if (size != std::string::npos) {
				unsigned int buff_int = std::stoi(str, &size);
				result = obj->client_base.find(buff_int);
			}
			else {
				bool buff_bool = std::stoi(str, &size);
				result = obj->client_base.find(buff_bool);
			}

		}
		size = str.find(",");
		str = str.substr(size + 1, std::string::npos);

		if (result.empty()) {
			obj->log->save(obj, 3);
			throw std::runtime_error("\nNone client was found");
		}

		while (size != std::string::npos) {
			size = str.find(",");
			buff_str = str.substr(0, size);
			if (buff_str.empty()) {
				obj->log->save(obj, 2);
				throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
			}
			std::size_t buff_size = buff_str.find_first_not_of("0123456789");
			if (buff_size != std::string::npos) {
				result = obj->client_base.find(result, buff_str);
			}
			else {
				std::size_t buff_size = buff_str.find_first_not_of("01");
				if (buff_size != std::string::npos) {
					unsigned int buff_int = std::stoi(str, &buff_size);
					result = obj->client_base.find(result, buff_int);
				}
				else {
					bool buff_bool = std::stoi(str, &buff_size);
					result = obj->client_base.find(result, buff_bool);
				}
			}
			if (result.empty()) {
				obj->log->save(obj, 2);
				throw std::runtime_error("\nNone client was found");
			}
			if (size != std::string::npos)
				str = str.substr(size + 1, std::string::npos);
		}

		if (result.empty()) {
			obj->log->save(obj, 2);
			throw std::runtime_error("\nNone client was found");
		}

		for (int i = 0; i < result.size(); i++) {
			Client buff = obj->client_base.get(result.at(i));
			std::cout << buff;
		}
	}
	obj->log->save(obj, 0);
}
void command_manager::Command::find_seats_func(std::string& str, Command* obj) {
	obj->set_function_name("find_seats_func");
	cleaner(str);
	std::cout << "\n";

	std::size_t size = str.find_first_not_of("1234567890.,");
	if (size != std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid input\nSee 'help' for more information");
	}
	size = str.find(",");
	if (size == std::string::npos) {
		std::cout << std::endl;
		unsigned int buff_int = std::stoi(str, &size);
		auto find_result = obj->seats_base.find(buff_int);
		if (!find_result.empty()) {
			for (int i = 0; i < find_result.size(); i++) {
				Seats seats = obj->seats_base.get(find_result.at(i));
				std::cout << seats;
			}
		}
		else {
			obj->log->save(obj, 5);
			throw std::runtime_error("\nNone was found");
		}
	}
	else {
		std::string buff_str = str.substr(0, size);
		if (buff_str.empty()) {
			obj->log->save(obj, 2);
			throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
		}

		str = str.substr(size + 1, std::string::npos);

		unsigned int buff_int = std::stoi(buff_str, &size);
		auto find_result = obj->seats_base.find(buff_int);

		while (size != std::string::npos) {
			size = str.find(",");
			buff_str = str.substr(0, size);
			if (buff_str.empty()) {
				obj->log->save(obj, 2);
				throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
			}

			buff_int = std::stoi(buff_str, &size);
			find_result = obj->seats_base.find(find_result, buff_int);

			size = str.find(",");
			if (find_result.empty()) {
				obj->log->save(obj, 5);
				throw std::runtime_error("\nNone was found");
			}
			if (size != std::string::npos)
				str = str.substr(size + 1, std::string::npos);
		}
		for (int i = 0; i < find_result.size(); i++) {
			Seats seats = obj->seats_base.get(find_result.at(i));
			std::cout << seats;
		}
	}
	obj->log->save(obj, 0);
}
void command_manager::Command::calculate_func(std::string& str, Command* obj) {
	obj->set_function_name("calculate_func");
	cleaner(str);
	std::size_t size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}

	std::string buff_str = str.substr(0, size);
	if (buff_str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	std::size_t buff_size = buff_str.find_first_not_of("0123456789");
	if (buff_size != std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	unsigned int buff_int = std::stoi(buff_str, &size);
	auto find_result = obj->bus_base.find(buff_int);
	if (find_result.empty()) {
		obj->log->save(obj, 4);
		throw std::runtime_error("\nThere isn't any bus this such id");
	}
	str = str.substr(size + 1, std::string::npos);

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	buff_str = str.substr(0, size);
	if (buff_str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	str = str.substr(size + 1, std::string::npos);

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	std::string end = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);
	if (str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	size = str.find_first_not_of("01");
	if (size != std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}

	buff_int = obj->bus_base.get(find_result.at(0)).calculate(buff_str, end, str);
	std::cout << "\nCost for client: " << buff_int << "\n";
	obj->log->save(obj, 0);
}

void command_manager::Command::sort_client_func(std::string& str, Command* obj) {
	obj->set_function_name("sort_client_func");

	if (str == "date") {
		obj->sort_client = str;

		for (unsigned int i = 0; i < obj->client_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->client_base.get_capacity() - 1; j++) {
				pair_int first;
				first.first = std::stoi(obj->client_base.get(j).get_date().first);
				first.second = std::stoi(obj->client_base.get(j).get_date().second);

				pair_int second;
				second.first = std::stoi(obj->client_base.get(j + 1).get_date().first);
				second.second = std::stoi(obj->client_base.get(j + 1).get_date().second);

				if (first.second < second.second) {
					obj->client_base.swap(j, j + 1);
				}
				if (first.first < second.first && first.second == second.second) {
					obj->client_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "name") {
		obj->sort_client = str;
		for (unsigned int i = 0; i < obj->client_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->client_base.get_capacity() - 1; j++) {
				if (obj->client_base.get(j).get_name() > obj->client_base.get(j + 1).get_name()) {
					obj->client_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "ticket") {
		obj->sort_client = str;
		for (unsigned int i = 0; i < obj->client_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->client_base.get_capacity() - 1; j++) {
				if (obj->client_base.get(j).get_ticket() < obj->client_base.get(j + 1).get_ticket()) {
					obj->client_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "benefit") {
		obj->sort_client = str;
		for (unsigned int i = 0; i < obj->client_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->client_base.get_capacity() - 1; j++) {
				if (obj->client_base.get(j).get_benefit() < obj->client_base.get(j + 1).get_benefit()) {
					obj->client_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "bus id") {
		obj->sort_client = str;
		for (unsigned int i = 0; i < obj->client_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->client_base.get_capacity() - 1; j++) {
				if (obj->client_base.get(j).get_bus_id() < obj->client_base.get(j + 1).get_bus_id()) {
					obj->client_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "reset") {
		obj->sort_client = "";
	}
	else {
		obj->log->save(obj, 7);
		throw std::runtime_error("\nInvalid sorting key\nSee 'help' for more information");
	}

	obj->log->save(obj, 0);
}
void command_manager::Command::sort_seats_func(std::string& str, Command* obj) {
	obj->set_function_name("sort_seats_func");

	if (str == "reset") {
		obj->sort_seats = "";
	}
	else if (str == "bus id") {
		obj->sort_seats = str;

		for (unsigned int i = 0; i < obj->seats_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->seats_base.get_capacity() - 1; j++) {
				if (obj->seats_base.get(j).get_id() < obj->seats_base.get(j + 1).get_id()) {
					obj->seats_base.swap(j, j + 1);
				}
			}
		}
	
	}
	else if (str == "date") {
		obj->sort_seats = "date";

		for (unsigned int i = 0; i < obj->seats_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->seats_base.get_capacity() - 1; j++) {
				pair_int first;
				first.first = std::stoi(obj->seats_base.get(j).get_date().first);
				first.second = std::stoi(obj->seats_base.get(j).get_date().second);

				pair_int second;
				second.first = std::stoi(obj->seats_base.get(j + 1).get_date().first);
				second.second = std::stoi(obj->seats_base.get(j + 1).get_date().second);

				if (first.second < second.second) {
					obj->seats_base.swap(j, j + 1);
				}
				if (first.first < second.first && first.second == second.second) {
					obj->seats_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "tickets") {
		obj->sort_seats = str;

		for (unsigned int i = 0; i < obj->seats_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->seats_base.get_capacity() - 1; j++) {
				if (obj->seats_base.get(j).get_size() < obj->seats_base.get(j + 1).get_size()) {
					obj->seats_base.swap(j, j + 1);
				}
			}
		}

	}
	else {
		obj->log->save(obj, 7);
		throw std::runtime_error("\nInvalid sorting key\nSee 'help' for more information");
	}

	obj->log->save(obj, 0);
}
void command_manager::Command::sort_bus_func(std::string& str, Command* obj) {
	obj->set_function_name("sort_bus_func");

	if (str == "bus id") {
		obj->sort_bus = str;
		for (unsigned int i = 0; i < obj->bus_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->bus_base.get_capacity() - 1; j++) {
				if (obj->bus_base.get(j).get_id() < obj->bus_base.get(j + 1).get_id()) {
					obj->bus_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "departure") {
		obj->sort_bus = str;
		for (unsigned int i = 0; i < obj->bus_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->bus_base.get_capacity() - 1; j++) {
				pair_int first;
				first.first = std::stoi(obj->bus_base.get(j).get_departure().first);
				first.second = std::stoi(obj->bus_base.get(j).get_departure().second);

				pair_int second;
				second.first = std::stoi(obj->bus_base.get(j + 1).get_departure().first);
				second.second = std::stoi(obj->bus_base.get(j + 1).get_departure().second);

				if (first.first < second.first) {
					obj->bus_base.swap(j, j + 1);
				}
				if (first.second < second.second && first.first == second.first) {
					obj->bus_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "arrival") {
		obj->sort_bus = str;
		for (unsigned int i = 0; i < obj->bus_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->bus_base.get_capacity() - 1; j++) {
				pair_int first;
				first.first = std::stoi(obj->bus_base.get(j).get_arrival().first);
				first.second = std::stoi(obj->bus_base.get(j).get_arrival().second);

				pair_int second;
				second.first = std::stoi(obj->bus_base.get(j + 1).get_arrival().first);
				second.second = std::stoi(obj->bus_base.get(j + 1).get_arrival().second);

				if (first.first < second.first) {
					obj->bus_base.swap(j, j + 1);
				}
				if (first.second < second.second && first.first == second.first) {
					obj->bus_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "cost") {
		obj->sort_bus = str;
		for (unsigned int i = 0; i < obj->bus_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->bus_base.get_capacity() - 1; j++) {
				if (obj->bus_base.get(j).get_cost() < obj->bus_base.get(j + 1).get_cost()) {
					obj->bus_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "max seats") {
		obj->sort_bus = str;
		for (unsigned int i = 0; i < obj->bus_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->bus_base.get_capacity() - 1; j++) {
				if (obj->bus_base.get(j).get_max_seats() < obj->bus_base.get(j + 1).get_max_seats()) {
					obj->bus_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "max benefit") {
		obj->sort_bus = str;
		for (unsigned int i = 0; i < obj->bus_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->bus_base.get_capacity() - 1; j++) {
				if (obj->bus_base.get(j).get_max_benefit_seats() < obj->bus_base.get(j + 1).get_max_benefit_seats()) {
					obj->bus_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "booked seats") {
		obj->sort_bus = str;
		for (unsigned int i = 0; i < obj->bus_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->bus_base.get_capacity() - 1; j++) {
				if (obj->bus_base.get(j).get_seats() < obj->bus_base.get(j + 1).get_seats()) {
					obj->bus_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "booked benefit") {
		obj->sort_bus = str;
		for (unsigned int i = 0; i < obj->bus_base.get_limit(); i++) {
			for (unsigned int j = 0; j < obj->bus_base.get_capacity() - 1; j++) {
				if (obj->bus_base.get(j).get_benefit_seats() < obj->bus_base.get(j + 1).get_benefit_seats()) {
					obj->bus_base.swap(j, j + 1);
				}
			}
		}
	}
	else if (str == "reset") {
		obj->sort_bus = "";
	}
	else {
		obj->log->save(obj, 7);
		throw std::runtime_error("\nInvalid sorting key\nSee 'help' for more information");
	}

	obj->log->save(obj, 0);
}

void command_manager::Command::add_client_func(std::string& str, Command* obj) {
	obj->set_function_name("add_client_func");
	cleaner(str);

	using pair = std::pair<std::string, std::string>;
	using pair_int = std::pair<int, int>;
	pair buff_time;
	buff_time.first = std::format("{:%d}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now()));
	buff_time.second = std::format("{:%m}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now()));
	pair_int buff_clock;
	std::size_t size = std::string::npos;
	buff_clock.first = std::stoi(std::format("{:%H}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now())), &size);
	buff_clock.second = std::stoi(std::format("{:%M}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now())), &size);

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	Client client;
	std::string buff_str = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);
	if (buff_str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}
	client.set_name(buff_str);

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	buff_str = str.substr(0, size);
	if (buff_str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	str = str.substr(size + 1, std::string::npos);
	unsigned int buff_int = std::stoi(buff_str, &size);
	auto find_result = obj->bus_base.find(buff_int);
	if (find_result.empty()) {
		obj->log->save(obj, 4);
		throw std::runtime_error("\nInvalid bus id");
	}
	client.set_bus_id(buff_int);

	size = str.find(",");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	buff_str = str.substr(0, size);
	if (buff_str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	str = str.substr(size + 1, std::string::npos);
	size = buff_str.find_first_not_of("01");
	if (size != std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	client.set_benefit(std::stoi(buff_str, &size));

	size = str.find(".");
	if (size == std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	buff_str = str.substr(0, size);
	if (buff_str.empty()) {
		obj->log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}

	str = str.substr(size + 1, std::string::npos);
	size = buff_str.find_first_not_of("1234567890");
	if (size != std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	size = str.find_first_not_of("1234567890");
	if (size != std::string::npos) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	size = std::string::npos;
	if (std::stoi(buff_str, &size) > 31 || std::stoi(str, &size) > 12) {
		obj->log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}

	client.set_date(buff_str, str);

	client.generate_ticket(5);

	pair buff_date;
	buff_date.first = buff_str;
	buff_date.second = str;

	std::vector<pos_type> buff_find_result;
	bool benefit = client.get_benefit();

	auto result = obj->bus_base.find(buff_int);
	Bus buff;
	if (!result.empty())
		buff = obj->bus_base.get(result.at(0));

	result = obj->seats_base.find(buff_int);
	if (!result.empty()) {
		result = obj->seats_base.find(result, buff_date);
		if (!result.empty()) {
			Seats seats_buff = obj->seats_base.get(result.at(0));
			if (!benefit) {
				if (unsigned int(seats_buff.get_size()) >= buff.get_max_seats() - buff.get_max_benefit_seats()) {
					obj->log->save("error (add_client_func): no non-benefit seats available");
					throw std::runtime_error("\nAll non-benefit seats has been booked");
				}
			}
			else {
				if (unsigned int(seats_buff.get_size()) >= buff.get_max_seats()) {
					obj->log->save("error (add_client_func): no seats available");
					throw std::runtime_error("\nAll seats has been booked");
				}
			}
		}
	}

	if (benefit) {
		bool checked = 0;
		buff_find_result = obj->client_base.find(buff_int);
		if (!buff_find_result.empty()) {
			buff_find_result = obj->client_base.find(buff_find_result, buff_date);
			if (!buff_find_result.empty()) {
				buff_find_result = obj->client_base.find(buff_find_result, benefit);
				checked = 1;
			}
		}
		if (buff_find_result.size() >= buff.get_max_benefit_seats() && checked == 1) {
			obj->log->save("error (add_client_func): no benefit seats available");
			throw std::runtime_error("\nAll benefit seats had been taken");
		}

	}

	buff_str = client.get_name();
	buff_find_result = obj->client_base.find(buff_str);
	if (buff_find_result.empty()) {
		pair_int buff_pair;
		buff_pair.first = std::stoi(obj->bus_base.get(find_result.at(0)).get_departure(0), &size);
		buff_pair.second = std::stoi(obj->bus_base.get(find_result.at(0)).get_departure(1), &size);
		bool executed = 0;

		pair_int buff_client_time;
		buff_client_time.first = std::stoi(client.get_date().first);
		buff_client_time.second = std::stoi(client.get_date().second);

		pair_int buff_int_time;
		buff_int_time.first = std::stoi(buff_time.first);
		buff_int_time.second = std::stoi(buff_time.second);

		if (buff_client_time == buff_int_time) {
			if (buff_clock.first <= buff_pair.first) {
				bool checked = 0;
				if (buff_pair.first != buff_clock.first)
					checked = 1;
				if (buff_clock.second < buff_pair.second || checked == 1) {

					obj->client_base.put(obj->client_base.size() + 1, client);
					obj->bus_base.get_ptr(find_result.at(0))->add_client(client.get_benefit());

					find_result = obj->seats_base.find(buff_int);

					if (!find_result.empty()) {
						pair buff = client.get_date();
						find_result = obj->seats_base.find(find_result, buff);
						if (find_result.empty()) {
							Seats seats;
							seats.set_id(client.get_bus_id());
							seats.put(client.get_ticket());
							seats.set_date(client.get_date().first, client.get_date().second);
							obj->seats_base.put(obj->client_base.size() + 1, seats);
						}
						else {
							Seats seats = obj->seats_base.get(find_result.at(0));
							seats.put(client.get_ticket());
							obj->seats_base.put(find_result.at(0), seats);
						}
					}
					else {
						Seats seats;
						seats.set_id(client.get_bus_id());
						seats.put(client.get_ticket());
						seats.set_date(client.get_date().first, client.get_date().second);
						obj->seats_base.put(obj->client_base.size() + 1, seats);
					}
					executed = 1;
				}
				else {
					obj->log->save("error (add_client_func): bus already departured");
					throw std::runtime_error("\nBus has already departured");
				}
			}
			else {
				obj->log->save("error (add_client_func): bus already departured");
				throw std::runtime_error("\nBus has already departured");
			}
		}
		else {
			std::cout << "\nHelp\n";
		}

		if (!executed) {
			obj->client_base.put(obj->client_base.size() + 1, client);
			find_result = obj->seats_base.find(buff_int);

			if (!find_result.empty()) {
				pair buff = client.get_date();
				find_result = obj->seats_base.find(find_result, buff);
				if (find_result.empty()) {
					Seats seats;
					seats.set_id(client.get_bus_id());
					seats.put(client.get_ticket());
					seats.set_date(client.get_date().first, client.get_date().second);
					obj->seats_base.put(obj->client_base.size() + 1, seats);
				}
				else {
					Seats seats = obj->seats_base.get(find_result.at(0));
					seats.put(client.get_ticket());
					obj->seats_base.put(find_result.at(0), seats);
				}
			}
			else {
				Seats seats;
				seats.set_id(client.get_bus_id());
				seats.put(client.get_ticket());
				seats.set_date(client.get_date().first, client.get_date().second);
				obj->seats_base.put(obj->client_base.size() + 1, seats);
			}
		}
	}
	else {
		obj->log->save("error (add_client_func): client with that name was found");
		throw std::runtime_error("\nClient with that name is already in database");
	}

	std::cout << "\nClient`s ticket: " << client.get_ticket() << "\n";
	obj->log->save(obj, 0);
}
void command_manager::Command::delete_client_func(std::string& str, Command* obj) {
	obj->set_function_name("delete_client_func");
	cleaner(str);

	pair buff_time;
	buff_time.first = std::format("{:%d}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now()));
	buff_time.second = std::format("{:%m}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now()));

	auto find_result = obj->client_base.find(str);
	if (find_result.size() == 0) {
		obj->log->save(obj, 3);
		throw std::runtime_error("\nThere isn`t any client with such name");
	}
	Client buff_client = obj->client_base.get(find_result.at(0));
	obj->client_base.erase(find_result.at(0));

	unsigned int buff_int = buff_client.get_ticket();
	find_result = obj->seats_base.find(buff_int);
	Seats buff_seat = obj->seats_base.get(find_result.at(0)).delete_client(buff_int);
	obj->seats_base.put(find_result.at(0), buff_seat);

	if (buff_client.get_date() == buff_time) {
		buff_int = buff_client.get_bus_id();
		find_result = obj->bus_base.find(buff_int);
		Bus buff_bus = obj->bus_base.get(find_result.at(0)).remove_client(buff_client.get_benefit());
		obj->bus_base.put(find_result.at(0), buff_bus);
	}
	obj->log->save(obj, 0);
}

void command_manager::Command::time_checker(Command* obj) {
	while (!obj->end()) {
		pair_int buff_time;
		std::mutex mutex;
		std::lock_guard<std::mutex> lock(mutex);
		buff_time.first = std::stoi(std::format("{:%d}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now())));
		buff_time.second = std::stoi(std::format("{:%m}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now())));

		pair_int buff_clock;
		std::size_t size = std::string::npos;
		buff_clock.first = std::stoi(std::format("{:%H}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now())), &size);
		buff_clock.second = std::stoi(std::format("{:%M}", std::chrono::current_zone()->to_local(std::chrono::system_clock::now())), &size);

		auto find_result = obj->seats_base.find(buff_time);
		if (!find_result.empty()) {
			for (int i = 0; i < find_result.size(); i++) {
				unsigned int buff_int = 0;
				unsigned int buff_id = obj->seats_base.get(find_result.at(buff_int)).get_id();
				auto result = obj->bus_base.find(buff_id);

				pair_int buff_arrival;
				buff_arrival.first = std::stoi(obj->bus_base.get(result.at(0)).get_arrival(0), &size);
				buff_arrival.second = std::stoi(obj->bus_base.get(result.at(0)).get_arrival(1), &size);

				if (buff_arrival.first <= buff_clock.first) {
					bool checked = 0;
					if (buff_arrival.first == buff_clock.first)
						checked = 1;
					if (buff_arrival.second < buff_clock.second || checked == 0) {

						Bus buff_bus = obj->bus_base.get(result.at(0)).remove_all_clients();
						obj->bus_base.put(result.at(0), buff_bus);

						result = obj->client_base.find(buff_id);
						if (!result.empty()) {
							result = obj->client_base.find(result, buff_time);
						}
						for (int j = 0; j < result.size(); j++)
							obj->client_base.erase(result.at(0));

						obj->seats_base.erase(find_result.at(buff_int));

					}
					else
						buff_int++;
				}
				else
					buff_int++;

			}

		}
		std::this_thread::sleep_for(std::chrono::minutes(1));
	}
}
void command_manager::Command::updater(Command* obj) {
	while (!obj->end()) {
		pair buff_time;
		std::mutex mutex;
		std::lock_guard<std::mutex> lock(mutex);
		auto date = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
		buff_time.first = std::format("{:%d}", date);
		buff_time.second = std::format("{:%m}", date);

		auto find_result = obj->seats_base.find(buff_time);
		if (find_result.empty()) {
			date += 24h;
			buff_time.first = std::format("{:%d}", date);
			buff_time.second = std::format("{:%m}", date);

			find_result = obj->seats_base.find(buff_time);
		}

		if (!find_result.empty()) {
			for (unsigned int i = 0; i < find_result.size(); i++) {
				unsigned int buff_int = obj->seats_base.get(find_result.at(i)).get_id();
				auto result = obj->bus_base.find(buff_int);
				if (result.empty())
					throw std::runtime_error("\nCritical error\nCheck bus database for id: " + buff_int);

				Bus buff = obj->bus_base.get(result.at(0));
				buff.set_seats(0);
				buff.set_benefit_seats(0);
				obj->bus_base.put(result.at(0), buff);

				for (int j = 0; j < obj->seats_base.get(find_result.at(i)).get_size(); j++) {
					unsigned int buff_int = j;
					unsigned int buff_ticket = obj->seats_base.get(find_result.at(i)).get(buff_int).get();
					auto client_find_result = obj->client_base.find(buff_ticket);
					if (client_find_result.empty())
						throw std::runtime_error("\nCritical error\nCheck client database for ticket: " + buff_ticket);

					Bus buff_bus = obj->bus_base.get(result.at(0));
					bool benefit = obj->client_base.get(client_find_result.at(0)).get_benefit();
					buff_bus.add_client(benefit);
					obj->bus_base.put(result.at(0), buff_bus);

				}

			}
		}
		std::this_thread::sleep_for(std::chrono::minutes(1));
	}

}

command_manager::Command::Command() {
	log = Log::create();
	if (admin_config.get_capacity() == 0) {
		admin_config.put(0, Admin_config{});
	}
	password = admin_config.get(0).get_password();
	bus_base.resize(admin_config.get_ptr(0)->get_bus());
	client_base.resize(admin_config.get_ptr(0)->get_client());
	seats_base.resize(admin_config.get_ptr(0)->get_seats());

	map.emplace("help", help_func);
	map.emplace("exit", exit_func);
	map.emplace("admin update", admin_update_func);
	map.emplace("leave admin", leave_admin_func);
	map.emplace("show all buses", show_all_buses_func);
	map.emplace("show all clients", show_all_clients_func);
	map.emplace("show all seats", show_all_seats_func);
	map.emplace("show all", show_all_func);
	map.emplace("clear", clear_func);
	map.emplace("guide", guide_func);

	map_variables.emplace("admin", admin_func);
	map_variables.emplace("admin add bus", admin_add_bus_func);
	map_variables.emplace("admin delete bus", admin_delete_bus_func);
	map_variables.emplace("find bus", find_bus_func);
	map_variables.emplace("calculate", calculate_func);
	map_variables.emplace("add client", add_client_func);
	map_variables.emplace("delete client", delete_client_func);
	map_variables.emplace("find client", find_client_func);
	map_variables.emplace("find seats", find_seats_func);
	map_variables.emplace("admin change password", admin_change_password_func);
	map_variables.emplace("admin resize bus database", admin_resize_bus_database_func);
	map_variables.emplace("admin resize client database", admin_resize_client_database_func);
	map_variables.emplace("admin resize seats database", admin_resize_seats_database_func);
	map_variables.emplace("sort client", sort_client_func);
	map_variables.emplace("sort bus", sort_bus_func);
	map_variables.emplace("sort seats", sort_seats_func);

	std::thread thr(time_checker, this);
	thr.detach();
	std::thread thr_upd(updater, this);
	thr_upd.detach();
}

command_manager::Command::~Command() = default;

const void command_manager::Command::operator>>(std::string& str) {
	log->save("user input: " + str);
	std::size_t size = str.find("(");
	bool executed = 0;

	if (size != std::string::npos) {
		std::size_t check_size = str.find(" (");
		if (check_size == std::string::npos) {
			log->save("error: user didn`t print 'space' between command and brackets OR unknown command");
			throw std::runtime_error("\nUnknown command\nSee 'help' for more information");
		}

		std::string variable = str.substr(size + 1, std::string::npos);
		str = str.substr(0, size - 1);

		check_size = variable.find(")");
		if (check_size == std::string::npos) {
			log->save("error: user didn`t close brackets");
			throw std::runtime_error("\nBrackets must be closed\nSee 'help' for more information");
		}
		variable.erase(check_size);

		auto it = map_variables.begin();
		do {
			if (it->first == str) {
				if (variable.empty()) {
					log->save("error: user didn`t print variables in brackets");
					throw std::runtime_error("\nBrackets must contain variable\nSee 'help' for more information");
				}
				it->second(variable, this);
				executed = 1;
				break;
			}
			it++;
		} while (it != map_variables.end());
	}
	else {
		auto it = map.begin();
		do {
			if (it->first == str) {
				it->second(this);
				executed = 1;
				break;
			}
			it++;
		} while (it != map.end());
	}
	if (!executed) {
		log->save("error: user printed unknown command");
		throw std::runtime_error("\nUnknown command\nSee 'help' for more information");
	}

}

const std::string command_manager::Command::get_message(const unsigned int msg) const {
	switch (msg) {
	case 0:
		return "success";
	case 1:
		return "missed syntax";
	case 2:
		return "missed value";
	case 3:
		return "client not found";
	case 4:
		return "bus not found";
	case 5:
		return "seat not found";
	case 6:
		return "user wasn't admin";
	case 7:
		return "invalid sorting key";
	default:
		return "undefined message";
	}
}

bool command_manager::Command::end() const { return this->exit; }