#include "Seats.h"

Seats::Seats() : Seats(0) {}
Seats::Seats(unsigned int id) : bus_id{ id } {}
Seats::~Seats() {};

void Seats::set_id(unsigned int& value) { bus_id = value; }
void Seats::set_id(unsigned int&& value) { bus_id = value; }
unsigned int Seats::get_id() const { return bus_id; }

std::size_t Seats::get_size() const { return clients.size(); }

void Seats::set_date(std::string& day, std::string& month) { date.first = day; date.second = month; }
void Seats::set_date(std::string&& day, std::string&& month) { date.first = day; date.second = month; }
pair Seats::get_date() const { return date; }

void Seats::put(unsigned int& data) { clients.emplace_back(Ticket(data)); }
void Seats::put(unsigned int&& data) { clients.emplace_back(Ticket(data)); }
void Seats::put(unsigned int& position, unsigned int& data) { clients.at(position) = data; }
void Seats::put(unsigned int& position, Ticket& data) { clients.at(position) = data; }
Ticket Seats::get(unsigned int& position) { return clients.at(position); }

Seats Seats::delete_client(unsigned int ticket) {
	auto it = this->clients.begin();
	while (it != this->clients.end()) {
		if (*it == ticket) {
			this->clients.erase(it);
			break;
		}
		it++;
	}
	log->save("success (Seats::delete_client)");
	return *this;
}

Seats& operator>>(std::string& str, Seats& obj) {
	std::size_t size = str.find("-");
	obj.date.first = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);
	size = str.find("#");
	obj.date.second = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	obj.bus_id = std::stoi(str, &size);
	str = str.substr(size + 1, std::string::npos);

	std::size_t buff_size = str.find("E");
	if (buff_size == std::string::npos) {
		unsigned int pos = 0;
		while (size != std::string::npos) {
			size = str.find("#");
			std::string buff_str = str.substr(0, size);
			unsigned int buff_int = std::stoi(buff_str, &size);
			obj.clients.emplace_back(buff_int);
			size = str.find("#");
			if (size != std::string::npos)
				str = str.substr(size + 1, std::string::npos);
		}
	}
	return obj;
}
std::ofstream& operator<<(std::ofstream& file, Seats& obj) {
	file << obj.date.first << "-" << obj.date.second << "#" << obj.bus_id << "#";
	for (int i = 0; i < obj.clients.size(); i++) {
		file << obj.clients.at(i).get();
		if (i != obj.clients.size() - 1)
			file << "#";
	}
	if (obj.clients.empty())
		file << "E";
	return file;
}

const bool Seats::operator==(const Seats& data) const {
	if (this->clients == data.clients && this->bus_id == data.bus_id)
		return true;
	return false;
}
const bool Seats::operator==(const unsigned int& data) const {
	if (this->bus_id == data)
		return true;
	for (int i = 0; i < this->clients.size(); i++)
		if (this->clients.at(i) == data)
			return true;
	std::string buff = std::to_string(data);
	if (buff == this->date.first || buff == this->date.second)
		return true;
	return false;
}
const bool Seats::operator==(const std::vector<Ticket>& data) const {
	return this->clients == data ? true : false;
}
const bool Seats::operator==(const Ticket& data) const {
	for (int i = 0; i < this->clients.size(); i++)
		if (this->clients.at(i) == data)
			return true;
	return false;
}
const bool Seats::operator==(const pair& data) const {
	return this->date == data ? true : false;
}
const bool Seats::operator==(const std::string data) const {
	if (this->date.first == data || this->date.second == data)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& os, Seats& obj) {
	os << "Date: " << obj.date.first << "." << obj.date.second << "\t"
		<< "Bus id: " << obj.bus_id << "\nTickets: ";
	for (int i = 0; i < obj.clients.size(); i++)
		os << obj.clients.at(i).get() << " ";

	os << "\n\n";
	return os;
}
