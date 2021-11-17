#include "JSONManager.hpp"

namespace JSONHandler {
	// Uses https://github.com/nlohmann/json

	json read(std::string filepath) {
		// Read from file
		json data;
		try {
			std::ifstream file(filepath);
			file >> data;
		}
		catch (const parse_error& error) {
			printf("No save file found!\n");
		}
		return data;
	}

	void write(std::string filepath, json data) {
		// Write to the file
		std::ofstream file(filepath);
		file << std::setw(4) << data << std::endl;
	}
}