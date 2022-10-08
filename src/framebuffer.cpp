#include "framebuffer.hpp"
#include "engineManager.hpp"

namespace engine {
	glFramebuffer::glFramebuffer(int width, int height)
	{
		unsigned int ID;
		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);

		this->mTexture = new glTexture(width, height, core::glTextureWrappingTypes::gl_clamp_to_edge, core::glTextureFilteringTypes::gl_nearest);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mTexture->getID(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			DebugConsole::error("Framebuffer failed to create!");
			this->mFramebufferID = 0;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return;
		}

		this->mFramebufferID = ID;

		base::bEngineManager::Instance->RegisterFramebuffer(this);
	}

	glFramebuffer::~glFramebuffer()
	{
		glDeleteFramebuffers(1, &this->mFramebufferID);
		delete this->mTexture;
	}

	void glFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->mFramebufferID);
	}

	void glFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
