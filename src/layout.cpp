#include "layout.hpp"

unsigned int engine::getLayoutStride(glMeshLayout layout)
{
	unsigned int result = 0;
	for (glLayout part : layout)
		result += getLayoutSize(part);
	return result;
}

unsigned int engine::getLayoutSize(glLayout layout)
{
	return util::enumAsInt((glLayoutType)layout.first) * layout.second;
}
