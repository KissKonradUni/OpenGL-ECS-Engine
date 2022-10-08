#include "entity.hpp"
#include "engineManager.hpp"

namespace base {

	// bComponent

	bool bComponent::getActive()
	{
		return this->mActive;
	}

	void bComponent::setActive(bool state)
	{
		this->mActive = state;
	}

	bEntity* bComponent::getEntity()
	{
		return mEntity;
	}

	// bEntity

	bEntity::bEntity()
	{
		this->mComponents = std::vector<bComponent*>();
	}

	bEntity::~bEntity()
	{
		for (bComponent* _ : this->mComponents)
			delete _;
		this->mComponents.clear();
		this->mComponents.~vector();
	}

	void bEntity::addComponent(bComponent* component)
	{
		component->setEntity(this);
		if (component->hasRequirements())
			this->mComponents.push_back(component);
		else {
			DebugConsole::error("This component cannot be added to the current entity, because it's missing some other mandatory components.");
			throw "Missing component!";
		}
	}

	void bEntity::render(float deltaTime)
	{
		for (bComponent* component : mComponents)
			component->render(deltaTime);
	}

	void bEntity::update(float deltaTime)
	{
		for (bComponent* component : mComponents)
			component->update(deltaTime);
	}

	void bEntity::updateFixed(float deltaTime)
	{
		for (bComponent* component : mComponents)
			component->updateFixed(deltaTime);
	}

	// bTransformComponent

	bTransformComponent::bTransformComponent(bTransformComponent* parent)
	{
		this->mParent = parent;

		this->mPosition = glm::vec3();
		this->mRotation = glm::vec3();
		this->mScale = glm::vec3(1.0f);

		this->mMatrix = glm::mat4x4(1);
	}

	bTransformComponent::~bTransformComponent()
	{
		this->mParent = nullptr;
	}

	void bTransformComponent::translate(glm::vec3 value)
	{
		this->mPosition += value;
	}

	void bTransformComponent::rotate(float degrees, glm::vec3 axis)
	{
		this->mRotation += glm::radians(axis) * glm::vec3(degrees);
	}

	void bTransformComponent::scale(glm::vec3 value)
	{
		this->mScale *= value;
	}

	void bTransformComponent::setPosition(glm::vec3 value)
	{
		this->mPosition = value;
	}

	void bTransformComponent::setRotation(glm::vec3 value)
	{
		this->mRotation = value;
	}

	void bTransformComponent::setScale(glm::vec3 value)
	{
		this->mScale = value;
	}

	glm::mat4x4 bTransformComponent::getModelMatrix(bool recalculate)
	{
		if (!recalculate)
			return mMatrix;

		mMatrix = glm::mat4x4(1.f);

		auto pos = glm::translate(glm::mat4x4(1.f), mPosition);
		auto rot = glm::rotate(glm::mat4x4(1.f), mRotation.x, glm::vec3(1.f, 0.f, 0.f));
			 rot = glm::rotate(rot, mRotation.y, glm::vec3(0.f, 1.f, 0.f));
			 rot = glm::rotate(rot, mRotation.z, glm::vec3(0.f, 0.f, 1.f));
		auto scale = glm::scale(glm::mat4x4(1.f), mScale);

		mMatrix *= pos * rot * scale;

		if (this->mParent != nullptr)
			mMatrix = mParent->getModelMatrix(false) * mMatrix;

		return mMatrix;
	}

	glm::vec3 bTransformComponent::getPosition()
	{
		return mPosition;
	}

	glm::vec3 bTransformComponent::getRotation()
	{
		return mRotation;
	}

	glm::vec3 bTransformComponent::getScale()
	{
		return mScale;
	}

	void bTransformComponent::setParent(bTransformComponent* parent)
	{
		this->mParent = parent;
	}

	bTransformComponent* bTransformComponent::getParent()
	{
		return this->mParent;
	}

	// bMaterialComponent

	bMaterialComponent::bMaterialComponent(std::vector<engine::glTexture*> textures, const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath)
	{
		this->mShader = new engine::glShader(vertexShaderPath, fragmentShaderPath, geometryShaderPath);
		this->mTextures = textures;
	}

	bMaterialComponent::~bMaterialComponent()
	{
		// TODO: Static cache picture data.
		//		for (engine::glTexture _ : this->mTextures)
		//			_.~glTexture();

		// TODO: Static cache shader programs.
		//		mShader.~glShader();
		
		for (engine::glTexture* texture : this->mTextures)
			delete texture;
		this->mTextures.clear();
		this->mTextures.~vector();
	}

	void bMaterialComponent::applyMaterial(std::function<void(engine::glShader*)> uniforms)
	{
		mShader->bind();
		for (int i = 0; i < this->mTextures.size(); i++) {
			mTextures[i]->bind(i);
			mShader->setUniform(std::string("texture" + std::to_string(i)).c_str(), (unsigned int)i);
		}
		uniforms(mShader);
	}

	// bRendererComponent

	bRendererComponent::bRendererComponent(core::glVertexArray vertices, core::glIndexArray indicies, engine::glMeshLayout layout, base::bEntity* camera)
	{
		this->mMesh = new engine::glMesh(vertices, indicies, layout);
		this->mCamera = camera;
		if (!camera->hasComponent<bCameraComponent>()) {
			DebugConsole::error("The provided camera entity does not have an active camera component.");
			throw "Invalid camera!";
		}
	}

	bRendererComponent::~bRendererComponent()
	{
		delete mMesh;
	}

	void bRendererComponent::setEntity(bEntity* entity)
	{
		bComponent::setEntity(entity);
		base::bEngineManager::Instance->RegisterRenderable(entity);
	}

	bool bRendererComponent::hasRequirements()
	{
		return (
			this->mEntity->hasComponent<bTransformComponent>() &&
			this->mEntity->hasComponent<bMaterialComponent>()
		);
	}

	void bRendererComponent::render(float deltaTime)
	{
		renderFrom(deltaTime, mCamera);
	}

	void bRendererComponent::renderFrom(float deltaTime, base::bEntity* camera)
	{
		auto modelMatrix = this->mEntity->getComponent<bTransformComponent>()->getModelMatrix();
		auto projectionMatrix = camera->getComponent<bCameraComponent>()->getProjectionMatrix();
		auto viewMatrix = camera->getComponent<bCameraComponent>()->getViewMatrix();
		auto viewPos = camera->getComponent<bTransformComponent>()->getPosition();

		this->mMesh->bind();
		this->mEntity->getComponent<bMaterialComponent>()->applyMaterial([modelMatrix, projectionMatrix, viewMatrix, viewPos](engine::glShader* shader) -> void {
			shader->setUniform("model", modelMatrix);
			shader->setUniform("projection", projectionMatrix);
			shader->setUniform("view", viewMatrix);
			shader->setUniform("viewPos", viewPos);
		});
		this->mMesh->render();
	}

	// bCameraComponent

	bCameraComponent::bCameraComponent(float fieldOfView, float aspect, float nearPlane, float farPlane)
	{
		this->mFieldOfView = fieldOfView;
		this->mAspectRatio = aspect;
		this->mNearPlane = nearPlane;
		this->mFarPlane = farPlane;

		this->mBaseMatrix = glm::perspective(glm::radians(mFieldOfView), mAspectRatio, mNearPlane, mFarPlane);

		this->mTransform = nullptr;
	}

	bool bCameraComponent::hasRequirements()
	{
		this->mTransform = this->mEntity->getComponent<bTransformComponent>();
		return mTransform != nullptr;
	}

	glm::vec3 bCameraComponent::getCameraFront()
	{
		return this->mCameraFront;
	}

	glm::vec3 bCameraComponent::getCameraUp()
	{
		return this->mCameraUp;
	}

	glm::vec3 bCameraComponent::getCameraRight()
	{
		return glm::cross(this->mCameraFront, this->mCameraUp);
	}

	void bCameraComponent::setCameraDirection(glm::vec3 dir)
	{
		this->mCameraFront = dir;
	}

	glm::mat4x4 bCameraComponent::getProjectionMatrix()
	{
		return this->mBaseMatrix;
	}

	glm::mat4x4 bCameraComponent::getViewMatrix()
	{
		auto cameraPos = mTransform->getPosition();
		return glm::lookAt(cameraPos, cameraPos + mCameraFront, mCameraUp);
	}

	void bCameraComponent::setAspectRatio(float aspect)
	{
		this->mAspectRatio = aspect;
		this->mBaseMatrix = glm::perspective(glm::radians(mFieldOfView), mAspectRatio, mNearPlane, mFarPlane);
	}
}