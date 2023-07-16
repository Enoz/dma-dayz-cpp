#pragma once
#include <iostream>
#include <sstream>
#include <vector>

namespace DMAMem {
	class Utils {
	public:
		static std::vector<std::string> split(std::string s, std::string delimiter);
	};
}