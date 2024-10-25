#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Logger.h"
#include "log.h"

class Abstract_handler {
private:
	Abstract_handler* next;
public:
	Log* log = Log::create();

	Abstract_handler();
	virtual ~Abstract_handler();

	Abstract_handler* set_next(Abstract_handler*);

	virtual bool handle(std::string, Logger*);

	static void delete_chain(Abstract_handler*);
};


class Amount_handler : public Abstract_handler {
public:
	bool handle(std::string, Logger*) override;
};


class Missed_value_handler : public Abstract_handler {
	bool handle(std::string, Logger*) override;
};


class End_handler : public Abstract_handler {
public:
	bool handle(std::string, Logger*) override;
};
