#pragma once
#include <type_traits>

namespace util {
	// Turns any enum into it's numerical value.
	// | Solution:
	// https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
	template <typename Enumeration>
	auto enumAsInt(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
	{
		return static_cast<typename std::underlying_type<Enumeration>::type>(value);
	}
}