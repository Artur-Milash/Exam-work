#pragma once
#include "Ticket.h"
#include <vector>
#include <fstream>
#include "Log.h"
#include "Colors.hpp"

using pair = std::pair<std::string, std::string>;

class Seats
{
private:
	std::vector<Ticket> clients;
	unsigned int bus_id;
	pair date;
	Log* log = Log::create();
public:
	Seats();
	Seats(unsigned int id);
	~Seats();
	Seats(const Seats&);

	void set_id(unsigned int& value);
	void set_id(unsigned int&& value);
	unsigned int get_id() const;

	std::size_t get_size() const;

	void set_date(std::string& day, std::string& month);
	void set_date(std::string&& day, std::string&& month);
	pair get_date() const;

	void put(unsigned int& data);
	void put(unsigned int&& data);
	void put(unsigned int& position, unsigned int& data);
	void put(unsigned int& position, Ticket& data);
	Ticket get(unsigned int& position);

	Seats delete_client(unsigned int ticket);

	friend Seats& operator>>(std::string& str, Seats& obj);
	friend std::ofstream& operator<<(std::ofstream& file, Seats& obj);

	const bool operator==(const Seats& data) const;
	const bool operator==(const unsigned int& data) const;
	const bool operator==(const std::vector<Ticket>& data) const;
	const bool operator==(const Ticket& data) const;
	const bool operator==(const pair& data) const;
	const bool operator==(const std::string data) const;
	const bool operator==(const std::pair<int, int>& data) const;

	friend std::ostream& operator<<(std::ostream& os, Seats& obj);
};