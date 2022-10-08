#pragma once
#include "includes.hpp"
#include "layout.hpp"

#include <vector>

namespace core {
	// A collection of floats representing a vertex buffer.
	typedef std::vector<float> glVertexArray;
	// A collection of the meshes indicies.
	typedef std::vector<unsigned int> glIndexArray;

	// The ID of the vertex buffer on the GPU.
	typedef unsigned int glVertexBufferObject;
	// The ID of the index buffer on the GPU.
	typedef unsigned int glElementBufferObject;
	// The ID of the vertex array on the GPU.
	typedef unsigned int glVertexArrayObject;

	// Generates and fills a VBO with the specified vertex buffer.
	glVertexBufferObject createVBO(glVertexArray vertices);
	// Generates and fills a EBO with the specified index buffer.
	glElementBufferObject createEBO(glIndexArray indicies);
	// Generates a VAO with the specified VBO and EBO, then assigns
	// the specified layout to it.
	glVertexArrayObject createVAO(glVertexBufferObject VBO, glElementBufferObject EBO, engine::glMeshLayout layout);
}

namespace engine {
	// A wrapper for the :core: objects to simplify
	// and hide unneccessary fields.
	// Also handles most of the rendering.
	class glMesh {
	public:
		glMesh(core::glVertexArray vertices, core::glIndexArray indicies, engine::glMeshLayout layout);
		~glMesh();

		// A basic render function.
		// The shader should be bound and 
		// filled with uniforms before calling.
		void render();

		// Binds the mesh for the current render cycle.
		void bind();
	private:
		core::glVertexBufferObject  mVBO;
		core::glElementBufferObject mEBO;
		core::glVertexArrayObject   mVAO;

		int mIndexSize;
	};
}