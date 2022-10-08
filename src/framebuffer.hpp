#pragma once

#include "includes.hpp"
#include "texture.hpp"
#include "logger.hpp"

namespace engine {

	class glFramebuffer
	{
	public:
		glFramebuffer(int width, int height);
		~glFramebuffer();

		void bind();
		void unbind();
	private:
		unsigned int mFramebufferID;
		glTexture*   mTexture;
	};

}