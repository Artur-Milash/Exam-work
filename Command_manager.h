#pragma once
#include <string>
#include <iostream>
#include "Client.h"
#include "Bus.h"
#include "Database.h"
#include "Seats.h"
#include <unordered_map>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include "Log.h"
#include "Admin_config.h"
#include "Logger.h"
#include "Colors.hpp"
#include "Handlers.h"

using pair = std::pair<std::string, std::string>;
using pair_int = std::pair<int, int>;
using namespace std::literals::chrono_literals;
using pos_type = unsigned long long;

class Command : public Logger {
private:
	Database<Admin_config> admin_config{ "Admin_config.txt", 1 };

	Database<Bus> bus_base{ "Bus_database.txt" };
	Database<Client> client_base{ "Client_database.txt" };
	Database<Seats> seats_base{ "Seats_database.txt" };

	bool exit = 0;
	bool admin = 0;
	std::string password;

	std::string sort_client;
	std::string sort_bus;
	std::string sort_seats;

	static void cleaner(std::string&);

	static void help_func(Command*);
	static void exit_func(Command*);
	static void clear_func(Command*);
	static void guide_func(Command*);

	static void admin_update_func(Command*);
	static void admin_func(std::string&, Command*);
	static void admin_add_bus_func(std::string&, Command*);
	static void admin_delete_bus_func(std::string&, Command*);
	static void leave_admin_func(Command*);
	static void admin_change_password_func(std::string&, Command*);
	static void admin_resize_bus_database_func(std::string&, Command*);
	static void admin_resize_client_database_func(std::string&, Command*);
	static void admin_resize_seats_database_func(std::string&, Command*);

	static void show_all_buses_func(Command*);
	static void show_all_clients_func(Command*);
	static void show_all_seats_func(Command*);
	static void show_all_func(Command*);

	static void find_bus_func(std::string&, Command*);
	static void find_client_func(std::string&, Command*);
	static void find_seats_func(std::string&, Command*);
	static void calculate_func(std::string&, Command*);

	static void sort_client_func(std::string&, Command*);
	static void sort_seats_func(std::string&, Command*);
	static void sort_bus_func(std::string&, Command*);

	static void add_client_func(std::string&, Command*);
	static void delete_client_func(std::string&, Command*);

	static void time_checker(Command*);
	static void updater(Command*);

	const std::string get_message(const unsigned int) const override;

	//add copy and move constructors

	std::unordered_map<std::string, void(*)(Command*)> map;
	std::unordered_map<std::string, void(*)(std::string&, Command*)> map_variables;

public:
	Log* log = Log::create();

	Command();
	~Command();

	const void operator>>(std::string&);

	bool end() const;
};
