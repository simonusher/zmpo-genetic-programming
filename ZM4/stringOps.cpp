#include "stringOps.h"

bool stringOps::isNumber(const std::string &string) {
	return string.find_first_not_of(NUMERIC_CHARACTERS_STRING) == std::string::npos;
}

std::string stringOps::removeLeadingZeros(const std::string &oldString) {
	int firstNotZeroIndex = oldString.find_first_not_of("0");
	if (firstNotZeroIndex == std::string::npos) {
		return EMPTY_STRING;
	}
	else {
		return oldString.substr(firstNotZeroIndex, oldString.length() - firstNotZeroIndex);
	}
}

bool stringOps::argumentOverflowsInt(std::string &argument) {
	//check string length for overflow
	if (argument.size() < MAX_INT_STRING_LENGTH) {
		return false;
	}
	if (argument.size() > MAX_INT_STRING_LENGTH) {
		return true;
	}
	else {
		bool overflows = false;
		std::string::const_iterator it = argument.begin();
		std::string::const_iterator maxIntIt = MAX_INT_STRING.begin();
		while (it != argument.end() && !overflows) {
			if ((*maxIntIt) < (*it)) {
				overflows = true;
			}
			++it;
			++maxIntIt;
		}
		return overflows;
	}
}