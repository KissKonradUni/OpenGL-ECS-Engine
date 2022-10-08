#pragma once
#include "includes.hpp"

#include <vector>

namespace engine {
	// The possible types of data in the vertex buffer.
	enum glLayoutType {
		gl_float = sizeof(float),
		gl_unsigned_int = sizeof(int),
		gl_bool = sizeof(bool)
	};

	// A pair class containing the type of the data(first) and the length of it(second).
	typedef std::pair<glLayoutType, uint8_t> glLayout;
	// A collection of layout parts.
	typedef std::vector<glLayout> glMeshLayout;

	// Gets the whole length of one vertex.
	unsigned int getLayoutStride(glMeshLayout layout);
	// Gets the length of one layout part.
	unsigned int getLayoutSize(glLayout layout);
}