#include "Handlers.h"

Abstract_handler::Abstract_handler() : next{ nullptr } {}
Abstract_handler::~Abstract_handler() = default;

Abstract_handler* Abstract_handler::set_next(Abstract_handler* obj) {
	this->next = obj;
	return obj;
}

bool Abstract_handler::handle(std::string str, Logger* obj) {
	if (this->next) {
		return this->next->handle(str, obj);
	}
	return 0;
}

void Abstract_handler::delete_chain(Abstract_handler* obj) {
	if (obj->next) {
		Abstract_handler* it = obj;
		std::vector<Abstract_handler*> buff;

		while (it->next) {
			buff.emplace_back(it);
			it = it->next;
		}
		buff.emplace_back(it);

		for (Abstract_handler* value : buff) {
			delete value;
		}
	}
	else {
		delete obj;
	}
}


bool Amount_handler::handle(std::string str, Logger* obj) {
	std::size_t size = str.find(",");

	if (size != std::string::npos) {
		return Abstract_handler::handle(str.substr(size + 1, std::string::npos), obj);
	}
	else {
		log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nSee 'help' for more information");
	}
}


bool Missed_value_handler::handle(std::string str, Logger* obj) {
	std::size_t size = str.find(",");

	if (!str.substr(0, size).empty()) {
		return Abstract_handler::handle(str, obj);
	}
	else {
		log->save(obj, 2);
		throw std::runtime_error("\nInvalid syntax\nMissed value\nSee 'help' for more information");
	}
}


bool End_handler::handle(std::string str, Logger* obj) {
	std::size_t size = str.find(",");

	if (size == std::string::npos) {
		return 1;
	}
	else {
		log->save(obj, 1);
		throw std::runtime_error("\nInvalid syntax\nToo much arguments\nSee 'help' for more information");
	}
}
