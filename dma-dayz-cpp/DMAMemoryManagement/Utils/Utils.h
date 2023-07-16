#pragma once
#include <iostream>
#include <sstream>
#include <vector>

namespace DMAUtils {
	std::vector<std::string> split(std::string s, std::string delimiter);

	template <typename T>
	void concatVectors(std::vector<T>* dest, std::vector<T>* src) {
		dest->insert(
			dest->end(),
			std::make_move_iterator(src->begin()),
			std::make_move_iterator(src->end())
		);
	}
}