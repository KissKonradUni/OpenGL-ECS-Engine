#include "texture.hpp"
#include "logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace core;

core::glTexture2D core::createTexture2D(unsigned char* data, int width, int height, glTextureWrappingTypes wrapping, glTextureFilteringTypes filtering, bool useMipmaps)
{
	unsigned int id;
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	if (useMipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	return id;
}

namespace engine {
	glTexture::glTexture(const char* filename, glTextureWrappingTypes wrapping, glTextureFilteringTypes filtering, bool useMipmaps)
	{
		stbi_set_flip_vertically_on_load(true);

		int width, height, colorChannels;
		unsigned char* data = stbi_load(filename, &width, &height, &colorChannels, 0);

		if (!data) {
			DebugConsole::error(std::string("Failed to load texture \"" + std::string(filename) + "\"").c_str());
			this->mTextureID = 0;
			return;
		}

		this->mTextureID = core::createTexture2D(data, width, height, wrapping, filtering, useMipmaps);

		stbi_image_free(data);
	}

	glTexture::glTexture(int width, int height, core::glTextureWrappingTypes wrapping, core::glTextureFilteringTypes filtering, bool useMipmaps)
	{
		this->mTextureID = core::createTexture2D(NULL, width, height, wrapping, filtering, useMipmaps);
	}

	glTexture::glTexture(uint8_t red, uint8_t green, uint8_t blue, core::glTextureWrappingTypes wrapping, core::glTextureFilteringTypes filtering, bool useMipmaps)
	{
		unsigned char data[3] = {red, green, blue};
		this->mTextureID = core::createTexture2D(&data[0], 1, 1, wrapping, filtering, useMipmaps);
		delete data;
	}

	glTexture::~glTexture()
	{
		glDeleteTextures(1, &mTextureID);
	}

	void glTexture::bind(uint8_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
	}

	void glTexture::resize(int width, int height)
	{
		glBindTexture(GL_TEXTURE_2D, this->mTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	core::glTexture2D glTexture::getID()
	{
		return this->mTextureID;
	}
}
