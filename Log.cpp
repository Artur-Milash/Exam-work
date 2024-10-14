#include "Log.h"

std::string* Log::str_ptr = nullptr;
Log* Log::ptr = nullptr;

Log::Log(std::string str) {
	str_ptr = new std::string{ str };
}
Log::~Log() = default;

void Log::release() {
	delete str_ptr;
	str_ptr = nullptr;
	delete ptr;
	ptr = nullptr;
}

Log* Log::create() {
	if (ptr == nullptr) {
		std::filesystem::create_directory("Logs");
		std::string data = std::format("{:%d-%m-%y-%H_%M_%S}", std::chrono::current_zone()
			->to_local(std::chrono::system_clock::now()));
		ptr = new Log{"Logs/Log#" + data + ".txt"};
		std::ofstream create_file(*str_ptr);
	}
	return ptr;
}

void Log::save(const std::string msg) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	std::ofstream file(*str_ptr, std::ios::app);
	file << std::format("{:%X}", std::chrono::current_zone()
		->to_local(std::chrono::system_clock::now()))
		<< " || " << msg << std::endl;
}

void Log::save(const Logger* obj, const unsigned int&& msg) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	std::ofstream file(*str_ptr, std::ios::app);
	file << std::format("{:%X}", std::chrono::current_zone()
		->to_local(std::chrono::system_clock::now()))
		<< " || " << obj->get_function_name() << " | " << obj->get_message(msg) << std::endl;
}

void Log::save(const Logger* obj, const std::string msg) {
	std::mutex mutex;
	std::lock_guard<std::mutex> lock(mutex);
	std::ofstream file(*str_ptr, std::ios::app);
	file << std::format("{:%X}", std::chrono::current_zone()
		->to_local(std::chrono::system_clock::now()))
		<< " || " << obj->get_function_name() << " | " << msg << std::endl;
}