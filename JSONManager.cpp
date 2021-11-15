#include "JSONManager.hpp"

namespace JSONHandler {
	// Uses https://github.com/nlohmann/json

	json read(std::string path, std::string filename) {
		// Read from file
		json data;
		std::ifstream file(path + "/" + filename + ".json");
		file >> data;

		return data;
	}

	void write(std::string path, std::string filename, json data) {
		// Write to the file
		std::ofstream file(path + "/" + filename + ".json");
		file << std::setw(4) << data << std::endl;
	}
}