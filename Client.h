#pragma once
#include <string>
#include "Ticket.h"
#include <fstream>
#include "Log.h"
#include "Colors.hpp"

using pair = std::pair<std::string, std::string>;

class Client
{
private:
	std::string name;
	Ticket ticket;
	unsigned int bus_id;
	bool have_benefit;
	pair date;
	Log* log = Log::create();
public:
	Client();
	Client(std::string name_value);
	Client(std::string name_value, unsigned int ticket_value);
	Client(std::string name_value, unsigned int ticket_value, unsigned int bus_value);
	Client(std::string name_value, unsigned int ticket_value, unsigned int bus_value, bool value);
	~Client();

	void set_date(std::string& day, std::string& month);
	void set_date(std::string&& day, std::string&& month);
	pair get_date() const;

	void set_bus_id(unsigned int& value);
	void set_bus_id(unsigned int&& value);
	unsigned int get_bus_id() const;

	void set_name(std::string& new_name);
	std::string get_name();

	void set_ticket(unsigned int& new_ticket);
	void generate_ticket(unsigned int amount);
	unsigned int get_ticket();

	void set_benefit(bool& value);
	void set_benefit(bool&& value);
	bool get_benefit() const;

	friend Client& operator>>(std::string& str, Client& client);
	friend std::ofstream& operator<<(std::ofstream& file, Client& client);

	void operator=(const Client& obj);
	const bool operator==(const Client& data) const;
	const bool operator==(const unsigned int& data) const;
	const bool operator==(const std::string& data) const;
	const bool operator==(const pair& data) const;
	const bool operator==(const Ticket& data) const;
	const bool operator==(const bool& data) const;

	friend std::ostream& operator<<(std::ostream& os, Client& obj);
};