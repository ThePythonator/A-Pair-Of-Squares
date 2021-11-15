// https://github.com/nlohmann/json
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <string>

namespace JSONHandler {
	using namespace nlohmann;

	json read(std::string path, std::string filename);
	void write(std::string path, std::string filename, json data);
}