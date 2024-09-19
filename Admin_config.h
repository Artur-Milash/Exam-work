#pragma once
#include <string>
#include <fstream>
#include "Log.h"

class Admin_config
{
private:
	using limit_type = unsigned long long;

	std::string password;
	limit_type limit_bus_database;
	limit_type limit_client_database;
	limit_type limit_seats_database;

	Log* log = Log::create();
public:
	Admin_config();
	Admin_config(const limit_type&, const limit_type&, const limit_type&);
	Admin_config(const limit_type&, const limit_type&, const limit_type&, const std::string&);

	~Admin_config();

	limit_type get_bus() const;
	limit_type get_client() const;
	limit_type get_seats() const;
	std::string get_password() const;

	void set_bus(const limit_type&);
	void set_client(const limit_type&);
	void set_seats(const limit_type&);
	void set_password(const std::string&);

	friend std::ofstream& operator<<(std::ofstream&, Admin_config&);
	friend Admin_config& operator>>(std::string&, Admin_config&);
};

