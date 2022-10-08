#pragma once
#include "includes.hpp"

namespace core {
	// OpenGL texture wrapping types.
	enum glTextureWrappingTypes {
		gl_repeat	       = GL_REPEAT,
		gl_mirrored_repeat = GL_MIRRORED_REPEAT,
		gl_clamp_to_edge   = GL_CLAMP_TO_EDGE,
		gl_clamp_to_border = GL_CLAMP_TO_BORDER
	};

	// OpenGL texture filtering types.
	enum glTextureFilteringTypes {
		gl_nearest  = GL_NEAREST,
		gl_bilinear = GL_LINEAR
	};

	// An identifier for an OpenGL texture.
	typedef unsigned int glTexture2D;

	// Creates an OpenGL texture from the specified 
	// data with the specified settings.
	// Returns the identifier of the texture.
	glTexture2D createTexture2D(unsigned char* data, int width, int height, glTextureWrappingTypes wrapping, glTextureFilteringTypes filtering, bool useMipmaps);
}

namespace engine {
	// A wrapper class for loading and binding textures.
	class glTexture
	{
	public:
		glTexture(const char* filename, core::glTextureWrappingTypes wrapping, core::glTextureFilteringTypes filtering, bool useMipmaps = true);
		glTexture(int width, int height, core::glTextureWrappingTypes wrapping, core::glTextureFilteringTypes filtering, bool useMipmaps = false);
		glTexture(uint8_t red, uint8_t green, uint8_t blue, core::glTextureWrappingTypes wrapping, core::glTextureFilteringTypes filtering, bool useMipmaps = false);
		~glTexture();

		// Binds the texture to the specified slot.
		// (Default = 0)
		void bind(uint8_t slot = 0);

		// Resizes the texture on the vram.
		// ! DELETES the contents of the texture because 
		// ! in order to resize the old buffer gets 
		// ! throw away and a new is created.
		void resize(int width, int height);

		// Gets the openGL ID of the texture.
		core::glTexture2D getID();
	private:
		core::glTexture2D mTextureID;
	};
}
