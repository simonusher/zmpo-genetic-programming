#pragma once
#include <string>
#include "constants.h"
const std::string NUMERIC_CHARACTERS_STRING = "0123456789";
namespace stringOps {
	bool isNumber(const std::string &string);
	bool argumentOverflowsInt(std::string &argument);
	std::string removeLeadingZeros(const std::string &oldString);
}