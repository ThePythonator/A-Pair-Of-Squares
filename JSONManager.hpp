#pragma once

// https://github.com/nlohmann/json
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <string>

namespace JSONHandler {
	using namespace nlohmann;
	using namespace nlohmann::detail;

	json read(std::string filepath);
	void write(std::string filepath, json data);
}