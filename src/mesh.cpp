#include "mesh.hpp"

// CORE

using namespace core;

glVertexBufferObject core::createVBO(glVertexArray vertices)
{
	unsigned int id = 0;
	glGenBuffers(1, &id);

	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	return id;
}

glElementBufferObject core::createEBO(glIndexArray indicies)
{
	unsigned int id = 0;
	glGenBuffers(1, &id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(int), &indicies[0], GL_STATIC_DRAW);

	return id;
}

glVertexArrayObject core::createVAO(glVertexBufferObject VBO, glElementBufferObject EBO, engine::glMeshLayout layout)
{
	unsigned int id = 0;
	glGenVertexArrays(1, &id);

	glBindVertexArray(id);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	unsigned int stride = engine::getLayoutStride(layout);
	unsigned int offset = 0;
	unsigned int index = 0;

	for (engine::glLayout part : layout) {
		glVertexAttribPointer(index, part.second, GL_FLOAT, GL_FALSE, stride, (void*)offset);
		offset += engine::getLayoutSize(part);
		glEnableVertexAttribArray(index++);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return id;
}

// GLMESH

namespace engine {
	glMesh::glMesh(glVertexArray vertices, glIndexArray indicies, engine::glMeshLayout layout) {
		this->mVBO = createVBO(vertices);
		this->mEBO = createEBO(indicies);
		this->mVAO = createVAO(mVBO, mEBO, layout);

		this->mIndexSize = indicies.size();
	}

	glMesh::~glMesh()
	{
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mVAO);
	}

	void glMesh::render()
	{
		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, mIndexSize, GL_UNSIGNED_INT, 0);
	}

	void glMesh::bind()
	{
		glBindVertexArray(mVAO);
	}
}