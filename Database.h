//For proper downloader and uploader work file must be written by the same type as database
// friend Class& operator>>(std::string& str, Class& obj) - must be overloaded
// friend std::ofstream& operator<<(std::ofstream& file, Class& Class) - must be overloaded
// method get() can`t be used with ostream operator<< and istream operator>> 
// const bool operator==(Type& data) const - must be overloaded for methods find()

#pragma once
#include <fstream>
#include <mutex>
#include <thread>
#include <string>
#include <vector>
#include "Log.h"
#include <stdio.h>

template <typename T>
class Database
{
private:
	using name_type = const std::string;
	using pos_type = unsigned long long;

	std::vector<T> cache;
	pos_type begin_pos;
	pos_type limit;
	name_type file_name;
	pos_type capacity;

	Log* log = Log::create();

	int select(std::string& str, int& data) { return std::stoi(str, 0); }
	long select(std::string& str, long& data) { return std::stol(str, 0); }
	long long select(std::string& str, long long& data) { return std::stoll(str, 0); }
	unsigned long select(std::string& str, unsigned long& data) { return std::stoul(str, 0); }
	pos_type select(std::string& str, pos_type& data) { return std::stoull(str, 0); }
	float select(std::string& str, float& data) { return std::stof(str, 0); }
	double select(std::string& str, double& data) { return std::stod(str, 0); }
	std::string select(std::string& str, std::string& data) { return str; }
	template <typename other>
	other select(std::string& str, other& data) {
		str >> data;
		return data;
	}

	static void download(Database* obj) {
		std::mutex mut;
		std::lock_guard<std::mutex> lock(mut);

		{ std::ofstream create_file{ obj->file_name, std::ios::app }; }

		std::ifstream file(obj->file_name);

		file.seekg(0, std::ios::end);
		if (file.tellg() != 0) {
			file.seekg(0, std::ios::beg);

			std::string str{};
			if (obj->begin_pos != 0) {
				for (pos_type buff_pos = 0; buff_pos < obj->begin_pos; buff_pos++)
					std::getline(file, str);
			}
			if (obj->capacity >= obj->limit) {
				for (pos_type i = 0; i < obj->limit; i++) {
					std::getline(file, str);
					if (str == std::string{})
						break;
					T data{};
					obj->cache.emplace_back(obj->select(str, data));
				}
			}
			else {
				for (pos_type i = 0; i < obj->capacity; i++) {
					std::getline(file, str);
					if (str == std::string{})
						break;
					T data{};
					obj->cache.emplace_back(obj->select(str, data));
				}
			}
		}
	}

	static void upload(Database* obj) {
		if (!obj->cache.empty()) {
			std::mutex mut;
			std::lock_guard<std::mutex> lock(mut);

			std::ifstream file_reader{ obj->file_name };

			name_type buff_name{ "buff_" + obj->file_name };
			std::ofstream buff_file_writer{ buff_name, std::ios::app };

			std::string str;
			for (pos_type i = 0; i < obj->begin_pos; i++) {
				std::getline(file_reader, str);
				buff_file_writer << str << std::endl;
			}
			for (int i = 0; i < obj->cache.size(); i++) {
				buff_file_writer << obj->cache.at(i) << std::endl;
				std::getline(file_reader, str);
			}
			for (pos_type i = obj->begin_pos + obj->limit; i < obj->capacity; i++) {
				std::getline(file_reader, str);
				buff_file_writer << str << std::endl;
			}

			buff_file_writer.close();
			file_reader.close();

			std::ofstream file_writer(obj->file_name);
			std::ifstream buff_file_reader(buff_name);

			for (pos_type i = 0; i < obj->capacity; i++) {
				std::getline(buff_file_reader, str);
				file_writer << str << std::endl;
			}
			buff_file_reader.close();
			std::remove(buff_name.c_str());
		}
	}

public:
	Database(const Database<T>&) = delete;
	Database(Database<T>&&) = delete;
	void operator=(const Database<T>&) = delete;

	Database(name_type& name) : Database(name, 500) {}
	Database(name_type& name, const pos_type& lim) : file_name{ name }, limit{ lim }, begin_pos{ 0 }, capacity{ 0 } {
		std::ifstream file(file_name);
		std::string str{};
		while (std::getline(file, str)) {
			capacity++;
		}
		file.close();
		download(this);
	}
	~Database() {
		upload(this);
	}

	void put(const pos_type pos, const T& data) {
		if (pos >= capacity) {
			if (begin_pos + limit <= capacity) {
				upload(this);
				cache.clear();
				begin_pos = (capacity + (limit / 2)) - limit;
				download(this);
			}
			cache.emplace_back(data);
			capacity++;
		}
		else if (pos >= begin_pos + limit) {
			upload(this);
			cache.clear();
			begin_pos = pos;
			download(this);
			cache.at(0) = data;
		}
		else if (pos >= begin_pos && pos < begin_pos + limit) {
			cache.at(pos - begin_pos) = data;
		}
		else if (pos >= 0 && pos < begin_pos) {
			upload(this);
			cache.clear();
			begin_pos = pos;
			download(this);
			cache.at(0) = data;
		}
		else {
			log->save("fail (Database::put): invalid position");
			throw std::runtime_error("\nCritical error! Invalid position in Database::put(...)\n");
		}
		log->save("success (Database::put)");
	}

	T get(const pos_type pos) {
		if (pos >= capacity) {
			log->save("fail (Database::get): position out of capasity");
			throw std::runtime_error("\nPosition out of capacity in Database::get(...)\n");
		}
		else if (pos >= begin_pos + limit) {
			upload(this);
			cache.clear();
			begin_pos = pos;
			download(this);
			return cache.at(0);
		}
		else if (pos >= begin_pos && pos < begin_pos + limit) {
			return cache.at(pos - begin_pos);
		}
		else if (pos >= 0 && pos < begin_pos) {
			upload(this);
			cache.clear();
			begin_pos = pos;
			download(this);
			return cache.at(0);
		}
		else {
			log->save("fail (Database::get): invalid position");
			throw std::runtime_error("\nCritical error! Invalid position in Database::get(...)\n");
		}
		log->save("success (Database::get)");
	}

	void erase(const pos_type pos) {
		if (pos >= capacity) {
			log->save("fail (Database::erase): position out of capacity");
			throw std::runtime_error("\nPosition out of capacity in Database::erase(...)\n");
		}
		else if (pos >= begin_pos + limit) {
			upload(this);
			cache.clear();
			begin_pos = pos;
			download(this);
			cache.erase(cache.begin());
			capacity--;
		}
		else if (pos >= begin_pos && pos < begin_pos + limit) {
			cache.erase(cache.begin() + (pos - begin_pos));
			capacity--;
		}
		else if (pos >= 0 && pos < begin_pos) {
			upload(this);
			cache.clear();
			begin_pos = pos;
			download(this);
			cache.erase(cache.begin());
			capacity--;
		}
		else {
			log->save("fail (Database::erase): invalid position");
			throw std::runtime_error("\nCritical error! Invalid position in Database::erase(...)\n");
		}
		log->save("success (Database::erase)");
	}

	T* get_ptr(const pos_type pos) {
		if (pos >= capacity) {
			log->save("fail (Database::get_ptr): position out of capacity");
			throw std::runtime_error("\nPosition out of capacity in Database::get_ptr(...)\n");
		}
		else if (pos >= begin_pos + limit) {
			upload(this);
			cache.clear();
			begin_pos = pos;
			download(this);
			return &cache.at(0);
		}
		else if (pos >= begin_pos && pos < begin_pos + limit) {
			return &cache.at(pos - begin_pos);
		}
		else if (pos >= 0 && pos < begin_pos) {
			upload(this);
			cache.clear();
			begin_pos = pos;
			download(this);
			return &cache.at(0);
		}
		else {
			log->save("fail (Database::get_ptr): invalid position");
			throw std::runtime_error("\nCritical error! Invalid position in Database::get_ptr(...)\n");
		}
		log->save("success (Database::get_ptr)");
	}

	void resize(const pos_type& new_limit) {
		if (new_limit > 0) {
			upload(this);
			cache.clear();
			this->limit = new_limit;
			download(this);
			log->save("success (Database::resize)");
		}
		else {
			log->save("fail (Database::resize): invalid value");
			throw std::runtime_error("\nInvalid value in Database::resize(...)\n");
		}
	}

	std::size_t size() const {
		return cache.size();
	}

	pos_type get_capacity() const {
		return capacity;
	}

	pos_type get_limit() const {
		return this->limit;
	}

	template <typename other>
	std::vector<pos_type> find(const other& key) {
		std::vector<pos_type> result{};
		for (pos_type i = 0; i < capacity; i++) {
			if (this->get(i) == key)
				result.emplace_back(i);
		}
		log->save("success (Database::find)");
		return result;
	}
	template <typename other>
	std::vector<pos_type> find(const std::vector<pos_type>& vect, const other& key) {
		if (!vect.empty()) {
			std::vector<pos_type> result;
			for (auto& value : vect) {
				if (this->get(value) == key)
					result.emplace_back(value);
			}
			log->save("success (Database::find)");
			return result;
		}
		else {
			log->save("fail (Database::find): vector is empty");
			throw std::runtime_error("\nCritical error! Vector is empty in Database::find(...)\n");
		}
	}

	void update() {
		std::thread thr{ upload, this };
		thr.detach();
	}
	void download() {
		download(this);
	}
};
