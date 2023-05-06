#include <iostream>
#include <fstream>
#include <string>

std::string readFile(const char* filePath) {

	std::string output;

	std::ifstream fileStream(filePath, std::ios::in);
	if (fileStream.is_open()) {
		std::string line;
		while (std::getline(fileStream, line)) {
			output.append(line);
			output.append("\n");
		}
	}

	return output;

}