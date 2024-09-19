#include "Admin_config.h"

using limit_type = unsigned long long;

Admin_config::Admin_config() : Admin_config(100, 500, 100, "admin") {};
Admin_config::Admin_config(const limit_type& lim_bus, const limit_type& lim_client,
	const limit_type& lim_seats) : Admin_config(lim_bus, lim_client, lim_seats, "admin") {}
Admin_config::Admin_config(const limit_type& lim_bus, const limit_type& lim_client,
	const limit_type& lim_seats, const std::string& pass)
	: password{ pass }, limit_bus_database{ lim_bus }, limit_client_database{ lim_client }, limit_seats_database{ lim_seats } {}

Admin_config::~Admin_config() = default;

limit_type Admin_config::get_bus() const { return limit_bus_database; }
limit_type Admin_config::get_client() const { return limit_client_database; }
limit_type Admin_config::get_seats() const { return limit_seats_database; }
std::string Admin_config::get_password() const { return password; }

void Admin_config::set_bus(const limit_type& lim) { 
	this->limit_bus_database = lim;
	log->save("(Admin_config::set_bus) setted bus limit to " + std::to_string(lim));
	log->save("success (Admin_config::set_bus)");
}
void Admin_config::set_client(const limit_type& lim) { 
	this->limit_client_database = lim;
	log->save("(Admin_config::set_client) setted client limit to " + std::to_string(lim));
	log->save("success (Admin_config::set_client)");
}
void Admin_config::set_seats(const limit_type& lim) { 
	this->limit_seats_database = lim;
	log->save("(Admin_config::set_seats) setted seats limit to " + std::to_string(lim));
	log->save("success (Admin_config::set_seats)");
}
void Admin_config::set_password(const std::string& pass) {
	this->password = pass;
	log->save("(Admin_config::set_password) setted password to " + pass);
	log->save("success (Admin_config::set_password)");
}


std::ofstream& operator<<(std::ofstream& file, Admin_config& obj) {
	file << obj.limit_bus_database << "#"
		<< obj.limit_client_database << "#"
		<< obj.limit_seats_database << "#"
		<< obj.password << '\n';
	return file;
}
Admin_config& operator>>(std::string& str, Admin_config& obj) {
	std::size_t size = str.find("#");

	obj.limit_bus_database = std::stoull(str.substr(0, size), &size);
	str = str.substr(size+1, std::string::npos);

	size = str.find("#");
	obj.limit_client_database = std::stoull(str.substr(0, size), &size);
	str = str.substr(size+1, std::string::npos);

	size = str.find("#");
	obj.limit_seats_database = std::stoull(str.substr(0, size), &size);
	str = str.substr(size+1, std::string::npos);

	obj.password = str;
	return obj;
}
