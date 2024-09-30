#include "Client.h"

Client::Client() : Client("None", 0, 0, 0) {}
Client::Client(std::string name_value) : Client(name_value, 0, 0, 0) {}
Client::Client(std::string name_value, unsigned int ticket_value) : Client(name_value, ticket_value, 0, 0) {}
Client::Client(std::string name_value, unsigned int ticket_value, unsigned int bus_value) : Client(name_value, ticket_value, bus_value, 0) {}
Client::Client(std::string name_value, unsigned int ticket_value, unsigned int bus_value, bool value)
	: name{ name_value }, ticket{ ticket_value }, bus_id{ bus_value }, have_benefit{ value } {}
Client::~Client() = default;

void Client::set_date(std::string& day, std::string& month) { date.first = day; date.second = month; }
void Client::set_date(std::string&& day, std::string&& month) { date.first = day; date.second = month; }
pair Client::get_date() const { return date; }

void Client::set_bus_id(unsigned int& value) { this->bus_id = value; }
void Client::set_bus_id(unsigned int&& value) { this->bus_id = value; }
unsigned int Client::get_bus_id() const { return this->bus_id; }

void Client::set_name(std::string& new_name) { name = new_name; }
std::string Client::get_name() { return name; }

void Client::set_ticket(unsigned int& new_ticket) { ticket.set(new_ticket); }
void Client::generate_ticket(unsigned int amount) {
	ticket.generate(amount);
	log->save("success (Client::generate_ticket)");
}
unsigned int Client::get_ticket() { return ticket.get(); }

void Client::set_benefit(bool& value) { this->have_benefit = value; }
void Client::set_benefit(bool&& value) { this->have_benefit = value; }
bool Client::get_benefit() const { return this->have_benefit; }

Client& operator>>(std::string& str, Client& client) {
	std::size_t size;

	size = str.find("#");
	client.name = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	client.bus_id = std::stoi(str, &size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	client.have_benefit = std::stoi(str, &size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	client.date.first = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	client.date.second = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);

	size = std::string::npos;
	client.ticket.set(std::stoi(str, &size));

	return client;
}
std::ofstream& operator<<(std::ofstream& file, Client& client) {
	file << client.name << "#"
		<< client.bus_id << "#"
		<< client.have_benefit << "#"
		<< client.date.first << "#" << client.date.second << "#"
		<< client.ticket.get();
	return file;
}

void Client::operator=(const Client& obj) {
	this->name = obj.name;
	this->bus_id = obj.bus_id;
	this->ticket = obj.ticket;
	this->have_benefit = obj.have_benefit;
	this->date = obj.date;
}
const bool Client::operator==(const Client& data) const {
	if (this->name == data.name && this->bus_id == data.bus_id
		&& this->ticket == data.ticket && this->have_benefit == data.have_benefit && this->date == data.date)
		return true;
	return false;
}
const bool Client::operator==(const unsigned int& data) const {
	unsigned int buff = this->ticket.get();
	if (this->bus_id == data || buff == data)
		return true;
	std::string buff_str = std::to_string(data);
	if (buff_str == this->date.first || buff_str == this->date.second)
		return true;
	return false;
}
const bool Client::operator==(const std::string& data) const {
	if (this->name == data || this->date.first == data || this->date.second == data)
		return true;
	return false;
}
const bool Client::operator==(const pair& data) const {
	if (this->date == data)
		return true;
	return false;
}
const bool Client::operator==(const Ticket& data) const {
	return this->ticket == data;
}
const bool Client::operator==(const bool& data) const {
	return this->have_benefit == data ? true : false;
}

std::ostream& operator<<(std::ostream& os, Client& obj) {
	os << high_red"Name: " << white<<obj.name
		<< high_green"     Ticket: " << white<< obj.ticket.get()
		<< high_magenta"     Bus id: "<< white << obj.bus_id
		<< high_yellow"     Benefit: " << white <<obj.have_benefit
		<< high_cyan"     Date: "<< white << obj.date.first << "." << obj.date.second << "\n\n";
	return os;
}
