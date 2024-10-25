#pragma once
#include <string>
#include "Randomized_class.h"
#include "Log.h"

class Ticket : public Randomized_class
{
private:
	unsigned int ticket;
	Log* log = Log::create();
public:
	Ticket();
	Ticket(unsigned int value);
	~Ticket();
	Ticket(const Ticket&);

	void generate(unsigned int char_amount) override;

	const bool operator==(const Ticket& data) const;
	const bool operator==(const unsigned int& data)const;
	void operator=(const unsigned int& data);
	void operator=(const Ticket data);

	void set(unsigned int value);
	unsigned int get() const;
};