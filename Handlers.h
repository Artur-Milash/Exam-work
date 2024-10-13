#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Command_manager.h"

class Abstract_handler {
private:
	Abstract_handler* next;

public:
	Abstract_handler();

	Abstract_handler* set_next(Abstract_handler*);

	virtual bool handle(std::string, command_manager::Command*);

	static void delete_chain(Abstract_handler*);
};


class Amount_handler : public Abstract_handler {
public:
	bool handle(std::string, command_manager::Command*) override;
};


class Missed_value_handler : public Abstract_handler {
	bool handle(std::string, command_manager::Command*) override;
};


class End_handler : public Abstract_handler {
public:
	bool handle(std::string, command_manager::Command*) override;
};
