#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "Log.h"
#include "Randomized_class.h"
#include "Colors.hpp"

class Bus : public Randomized_class
{
private:
	std::pair<std::string, std::string> arrival;
	std::pair<std::string, std::string> departure;
	unsigned int max_seats;
	unsigned int seats;
	unsigned int id;
	unsigned int cost;
	unsigned int benefit_seats;
	unsigned int max_benefit_seats;
	std::vector<std::string> route;
	Log* log = Log::create();
public:
	Bus();
	Bus(unsigned int max);
	Bus(unsigned int max, unsigned int numb);
	Bus(unsigned int max, unsigned int numb, unsigned int new_cost);
	Bus(unsigned int max, unsigned int numb, unsigned int new_cost, unsigned int benefit);
	Bus(const Bus& obj);
	~Bus();

	void set_max_benefit_seats(unsigned int& value);
	void set_max_benefit_seats(unsigned int&& value);
	unsigned int get_max_benefit_seats() const;

	void set_max_seats(unsigned int& value);
	void set_max_seats(unsigned int&& value);
	unsigned int get_max_seats() const;

	void set_seats(unsigned int& value);
	void set_seats(unsigned int&& value);
	unsigned int get_seats() const;

	void set_id(unsigned int& value);
	void set_id(unsigned int&& value);
	void generate(unsigned int char_amount) override;
	unsigned int get_id() const;

	void set_cost(unsigned int& value);
	void set_cost(unsigned int&& value);
	unsigned int get_cost() const;

	void set_benefit_seats(unsigned int& value);
	void set_benefit_seats(unsigned int&& value);
	unsigned int get_benefit_seats() const;

	void set_route(std::string& str);
	void set_route(std::string& str, unsigned int& pos);
	void set_route(std::string&& str, unsigned int&& pos);
	std::string get_route(unsigned int& pos) const;

	void set_arrival(std::string& hours, std::string& min);
	std::pair<std::string, std::string> get_arrival() const;
	std::string get_arrival(bool type) const;

	void set_departure(std::string& hours, std::string& min);
	std::pair<std::string, std::string> get_departure() const;
	std::string get_departure(bool type) const;

	unsigned int calculate(std::string& start, std::string& end, std::string& benefit);

	Bus add_client(bool benefit);
	Bus remove_client(bool benefit);
	Bus remove_all_clients();

	friend Bus& operator>>(std::string& str, Bus& bus);
	friend std::ofstream& operator<<(std::ofstream& file, Bus& bus);

	const bool operator==(const Bus& data) const;
	const bool operator==(const unsigned int& data) const;
	const bool operator==(const std::string& data) const;
	const bool operator==(const std::pair<std::string, std::string> pair) const;

	friend std::ostream& operator<<(std::ostream& os, Bus& obj);
};