#pragma once
#include <string>

class Logger
{
private:
	std::string function_name;
public:
	Logger() {};
	~Logger() = default;

	void set_function_name(const std::string& str) { this->function_name = str; }
	void set_function_name(const std::string&& str) { this->function_name = str; }
	const std::string get_function_name() const { return this->function_name; }

	virtual const std::string get_message(const unsigned int&) const = 0;
};

