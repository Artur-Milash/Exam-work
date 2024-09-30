#include "Bus.h"

Bus::Bus() : Bus(0, 0, 0, 0) {}
Bus::Bus(unsigned int max) : Bus(max, 0, 0, 0) {}
Bus::Bus(unsigned int max, unsigned int numb) : Bus(max, numb, 0, 0) {}
Bus::Bus(unsigned int max, unsigned int numb, unsigned int new_cost) : Bus(max, numb, new_cost, 0) {}
Bus::Bus(unsigned int max, unsigned int numb, unsigned int new_cost, unsigned int benefit)
	: max_seats{ max }, id{ numb }, cost{ new_cost }, max_benefit_seats{ benefit }, seats{ 0 }, benefit_seats{ 0 } {
	srand((unsigned int)std::time(0));
}
Bus::Bus(const Bus& obj) : arrival{ obj.arrival }, departure{ obj.departure },
max_seats{ obj.max_seats }, max_benefit_seats{ obj.max_benefit_seats },
seats{ obj.seats }, cost{ obj.cost }, benefit_seats{ obj.benefit_seats }, route{ obj.route }, id{obj.id} {
	srand((unsigned int)std::time(0));
}
Bus::~Bus() = default;

void Bus::set_max_benefit_seats(unsigned int& value) { max_benefit_seats = value; }
void Bus::set_max_benefit_seats(unsigned int&& value) { max_benefit_seats = value; }
unsigned int Bus::get_max_benefit_seats() const { return max_benefit_seats; }

void Bus::set_max_seats(unsigned int& value) { max_seats = value; }
void Bus::set_max_seats(unsigned int&& value) { max_seats = value; }
unsigned int Bus::get_max_seats() const { return max_seats; }

void Bus::set_seats(unsigned int& value) { this->seats = value; }
void Bus::set_seats(unsigned int&& value) { this->seats = value; }
unsigned int Bus::get_seats() const { return seats; }

void Bus::set_id(unsigned int& value) { id = value; }
void Bus::set_id(unsigned int&& value) { id = value; }
void Bus::generate(unsigned int char_amount){
	std::string str;
	unsigned int buff = 0;
	for (unsigned int i = 0; i < char_amount; i++) {
		buff = rand() % 10;
		str += std::to_string(buff);
	}
	std::size_t size = str.length();
	this->id = std::stoi(str, &size);
	log->save("success (Bus::generate)");
}
unsigned int Bus::get_id() const { return id; }

void Bus::set_cost(unsigned int& value) { this->cost = value; }
void Bus::set_cost(unsigned int&& value) { this->cost = value; }
unsigned int Bus::get_cost() const { return this->cost; }

void Bus::set_benefit_seats(unsigned int& value) { this->benefit_seats = value; }
void Bus::set_benefit_seats(unsigned int&& value) { this->benefit_seats = value; }
unsigned int Bus::get_benefit_seats() const { return this->benefit_seats; }

void Bus::set_route(std::string& str) { this->route.emplace_back(str); }
void Bus::set_route(std::string& str, unsigned int& pos) { route.at(pos) = str; }
void Bus::set_route(std::string&& str, unsigned int&& pos) { route.at(pos) = str; }
std::string Bus::get_route(unsigned int& pos) const { return route.at(pos); }

void Bus::set_arrival(std::string& hours, std::string& min) { arrival.first = hours; arrival.second = min; }
std::pair<std::string, std::string> Bus::get_arrival() const { return arrival; }
std::string Bus::get_arrival(bool type) const { return type == 0 ? this->arrival.first : this->arrival.second; }

void Bus::set_departure(std::string& hours, std::string& min) { departure.first = hours; departure.second = min; }
std::pair<std::string, std::string> Bus::get_departure() const { return departure; }
std::string Bus::get_departure(bool type) const { return type == 0 ? this->departure.first : this->departure.second; }

unsigned int Bus::calculate(std::string& start, std::string& end, std::string& benefit) {
	unsigned int start_pos = 0, end_pos = 0;
	bool exist = 0;
	for (int i = 0; i < route.size(); i++) {
		if (route.at(i) == start) {
			start_pos = i;
			exist = 1;
			break;
		}
	}
	if (!exist) {
		log->save("fail (Bus::calculate): city " + start + " doesn`t exist in this route");
		throw std::runtime_error("\nCity " + start + " doesn`t exist in this route");
	}
	exist = 0;
	for (int i = 0; i < route.size(); i++) {
		if (route.at(i) == end) {
			end_pos = i;
			exist = 1;
			break;
		}
	}
	if (!exist) {
		log->save("fail (Bus::calculate): city " + end + " doesn`t exist in this route");
		throw std::runtime_error("\nCity " + end + " doesn`t exist in this route");
	}
	if (start_pos > end_pos) {
		log->save("fail (Bus::calculate): invalid syntax");
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
	if (start_pos == end_pos) {
		log->save("fail (Bus::calculate): cities are equal");
		throw std::runtime_error("\nStart city and arrival city can`t be the same");
	}
	std::size_t buff = std::string::npos;
	if (!std::stoi(benefit, &buff)) {
		log->save("success (Bus::calculate)");
		if (start_pos == 0 && end_pos == route.size() - 1)
			return this->cost;
		return unsigned int((this->cost / this->route.size()) * (end_pos - start_pos));
	}
	else
	{
		log->save("success (Bus::calculate)");
		if (start_pos == 0 && end_pos == route.size() - 1)
			return this->cost / 2;
		return unsigned int(((this->cost / this->route.size()) * (end_pos - start_pos)) / 2);
	}
}

Bus Bus::add_client(bool benefit) {
	if (benefit) {
		if (benefit_seats != max_benefit_seats) {
			this->benefit_seats++;
			this->seats++;
		}
		else {
			log->save("fail (Bus::add_client): none benefit seats available");
			throw std::runtime_error("\nThere isn`t any benefit seats");
		}
	}
	else {
		if (seats < max_seats - max_benefit_seats)
			this->seats++;
		else {
			log->save("fail (Bus::add_client): bus is full");
			throw std::runtime_error("\nBus is full");
		}
	}
	log->save("success (Bus::add_client)");
	return *this;
}
Bus Bus::remove_client(bool benefit) {
	if (this->seats == 0) {
		log->save("fail (Bus::remove_client): bus is empty");
		throw std::runtime_error("\nBus is empty");
	}
	if (benefit) {
		if (this->max_benefit_seats != 0) {
			if (this->benefit_seats == 0) {
				log->save("fail (Bus::remove_client): none benefit seats");
				throw std::runtime_error("\nBus`s benefit seats is empty");
			}
			this->benefit_seats--;
		}
	}
	this->seats--;
	log->save("success (Bus::remove_client)");
	return *this;
}
Bus Bus::remove_all_clients() {
	this->seats = 0;
	this->benefit_seats = 0;
	log->save("success (Bus::remove_all_clients)");
	return *this;
}

Bus& operator>>(std::string& str, Bus& bus) {
	std::size_t size = str.find("#");

	bus.max_seats = std::stoi(str.substr(0, size), &size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	bus.seats = std::stoi(str.substr(0, size), &size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	bus.id = std::stoi(str.substr(0, size), &size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	bus.cost = std::stoi(str.substr(0, size), &size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	bus.benefit_seats = std::stoi(str.substr(0, size), &size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find("#");
	bus.max_benefit_seats = std::stoi(str.substr(0, size), &size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find(":");
	bus.arrival.first = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);
	size = str.find("#");
	bus.arrival.second = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);

	size = str.find(":");
	bus.departure.first = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);
	size = str.find("#");
	bus.departure.second = str.substr(0, size);
	str = str.substr(size + 1, std::string::npos);

	while (size != std::string::npos) {
		size = str.find("-");
		std::string buff_str = str.substr(0, size);
		bus.set_route(buff_str);
		if (size != std::string::npos)
			str = str.substr(size + 1, std::string::npos);
	}
	return bus;
}
std::ofstream& operator<<(std::ofstream& file, Bus& bus) {
	file << bus.max_seats << "#"
		<< bus.seats << "#"
		<< bus.id << "#"
		<< bus.cost << "#"
		<< bus.benefit_seats << "#"
		<< bus.max_benefit_seats << "#";

	file << bus.arrival.first << ":" << bus.arrival.second << "#"
		<< bus.departure.first << ":" << bus.departure.second << "#";

	for (int i = 0; i < bus.route.size(); i++) {
		file << bus.route.at(i);
		if (i != bus.route.size() - 1)
			file << "-";
	}
	return file;
}

const bool Bus::operator==(const Bus& data) const {
	if (this->max_seats == data.max_seats && this->seats == data.seats && this->id == data.id
		&& this->cost == data.cost && this->benefit_seats == data.benefit_seats && this->route == data.route
		&& this->arrival == data.arrival && this->departure == data.departure && this->max_benefit_seats == data.max_benefit_seats)
		return true;
	return false;
}
const bool Bus::operator==(const unsigned int& data) const {
	if (this->max_seats == data || this->seats == data || this->id == data
		|| this->benefit_seats == data || this->cost == data || this->max_benefit_seats == data)
		return true;
	std::string buff = std::to_string(data);
	if (buff == this->arrival.first || buff == this->arrival.second
		|| buff == this->departure.first || buff == this->departure.second)
		return true;
	return false;
}
const bool Bus::operator==(const std::string& data) const {
	for (int i = 0; i < this->route.size(); i++)
		if (this->route.at(i) == data)
			return true;
	if (this->arrival.first == data || this->arrival.second == data
		|| this->departure.first == data || this->departure.second == data)
		return true;
	return false;
}
const bool Bus::operator==(const std::pair<std::string, std::string> pair) const {
	if (pair == this->arrival || pair == this->departure)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& os, Bus& obj) {
	os << high_magenta"Bus id: " << white << obj.id
		<< high_cyan"     Departure: " << white << obj.departure.first << ":" << obj.departure.second
		<< high_cyan"     Arrival: " << white << obj.arrival.first << ":" << obj.arrival.second
		<< high_red"     Average cost: " << white<<obj.cost << std::endl;
	os << green"Max seats: " << white << obj.max_seats 
		<< "     Booked seats: " << obj.seats
		<< "     Available non-benefit seats: " << obj.max_seats - ((obj.seats - obj.benefit_seats) + obj.max_benefit_seats)
		<< "     Available seats: " <<obj.max_seats - obj.seats
		<< green"\nMax benefit seats: " << white <<obj.max_benefit_seats 
		<< "     Booked benefit seats: " << obj.benefit_seats
		<< "     Available benefit seats: "<<obj.max_benefit_seats - obj.benefit_seats << std::endl;
	os << high_yellow"Route:     ";
	for (int i = 0; i < obj.route.size(); i++) {
		os << white <<obj.route.at(i);
		if (i != obj.route.size() - 1)
			os << high_yellow" ---> " << white;
	}
	os << "\n\n";
	return os;
}
