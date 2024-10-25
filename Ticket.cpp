#include "Ticket.h"

Ticket::Ticket() : Ticket(0) {}
Ticket::Ticket(unsigned int value) : ticket{ value } { srand((unsigned int)time(NULL)); }
Ticket::~Ticket() = default;
Ticket::Ticket(const Ticket& obj) : ticket{ obj.ticket } { srand((unsigned int)std::time(0)); }

void Ticket::generate(unsigned int char_amount) {
	std::string str;
	unsigned int buff = 0;
	for (unsigned int i = 0; i < char_amount; i++) {
		buff = rand() % 10;
		str += std::to_string(buff);
	}
	std::size_t size = str.length();
	this->ticket = std::stoi(str, &size);
	log->save("success (Ticket::generate)");
}

const bool Ticket::operator==(const Ticket& data) const {
	if (this->ticket == data.ticket)
		return true;
	return false;
}
const bool Ticket::operator==(const unsigned int& data)const {
	return this->ticket == data ? true : false;
}
void Ticket::operator=(const unsigned int& data) {
	this->ticket = data;
}
void Ticket::operator=(const Ticket data) {
	this->ticket = data.ticket;
}

void Ticket::set(unsigned int value) { this->ticket = value; }
unsigned int Ticket::get() const { return this->ticket; }