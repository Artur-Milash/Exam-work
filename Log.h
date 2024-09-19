#pragma once
#include <string>
#include <chrono>
#include "fstream"
#include <filesystem>
#include <mutex>

class Log
{
private:
	static Log* ptr;
	static std::string* str_ptr;

	Log(std::string);
	~Log();
public:
	Log(const Log&) = delete;
	void operator=(const Log&) = delete;

	static void release();
	static Log* create();
	void save(const std::string&);
};