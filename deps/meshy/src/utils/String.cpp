#include <utils/String.h>
#include <sstream>
#include <ctime>

std::vector<std::string> SplitString(std::string const& value, char seperator) {
	std::vector<std::string> splitedStrings;
	size_t currentPos = 0;
	while (true) {
		size_t nextPos = value.find(seperator, currentPos);
		if (nextPos == std::string::npos) {
			std::string currentString = value.substr(currentPos);
			if (currentString.size())splitedStrings.push_back(currentString);
			break;
		}
		splitedStrings.push_back(value.substr(currentPos, nextPos - currentPos));
		currentPos = nextPos + 1;
	}
	return  splitedStrings;
}

std::string RandomString(std::string const& candidate, size_t length) {
	srand((uint32_t)time(0));
	std::string result;
	for (size_t i = 0; i < length; ++i) {
		result.push_back(candidate[rand() % candidate.size()]);
	}
	return result;
}

std::string itos(size_t number) {
	std::ostringstream oss;
	oss << number;
	return oss.str();
}

