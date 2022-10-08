#pragma once
#include "includes.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "mesh.hpp"

#include <vector>
#include <functional>

namespace base {
	// Forward declaration
	class bEntity;

	// A base component class.
	// It's virtual behaviour disallows
	// the direct use of it, but anything
	// extended from it can be used alongside
	// an entity.
	class bComponent {
	public:
		bComponent() { mEntity = nullptr; };
		virtual ~bComponent() {};

		// Checks if the entity has this component's required helpers.
		virtual bool hasRequirements() { return true; };

		// Called every frame when rendering.
		virtual void render(float deltaTime) {};
		// Called every frame after rendering.
		virtual void update(float deltaTime) {};
		// Called every gametick.
		virtual void updateFixed(float deltaTime) {};

		// Gets the active status of the component.
		bool getActive();
		// Sets the active status of the component.
		void setActive(bool state);

		// Sets a reference to the entity holding the component.
		virtual void setEntity(bEntity* entity) {
			this->mEntity = entity;
		};
		// Gets a reference to the entity holding the component.
		bEntity* getEntity();
	protected:
		bEntity* mEntity;
	private:
		bool mActive = true;
	};

	// A basic component containing the
	// position, rotation and scale
	// of the entity it's applied to.
	class bTransformComponent : public bComponent {
	public:
		bTransformComponent(bTransformComponent* parent = nullptr);
		~bTransformComponent();

		// Moves the object using the specified vector.
		void translate(glm::vec3 value);
		// Rotates the object using a degree and an axis.
		// The axis should be a normalized direction vector.
		void rotate(float degrees, glm::vec3 axis);
		// Scales the object using the input
		// vector as a multiplier.
		void scale(glm::vec3 value);

		// Sets the position of the entity.
		void setPosition(glm::vec3 value);
		// Sets the euler rotation of the entity.
		void setRotation(glm::vec3 value);
		// Sets the scale of the entity.
		void setScale(glm::vec3 value);

		// Gets the complete transformation matrix
		// for the object.
		glm::mat4x4 getModelMatrix(bool recalculate = true);

		// Position of the object.
		glm::vec3 getPosition();
		// Euler rotation of the object.
		glm::vec3 getRotation();
		// Scale of the object.
		glm::vec3 getScale();

		void setParent(bTransformComponent* parent);
		bTransformComponent* getParent();
	private:
		bTransformComponent* mParent;

		glm::mat4x4 mMatrix;

		glm::vec3 mPosition;
		glm::vec3 mRotation;
		glm::vec3 mScale;
	};

	// A simple material component.
	// Able to store all the neccesary
	// textures for the shader in order.
	// Special uniforms should be set with
	// the renderer component.
	class bMaterialComponent : public bComponent {
	public:
		bMaterialComponent(std::vector<engine::glTexture*> textures, const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = nullptr);
		~bMaterialComponent();

		// Applies the textures and the shader 
		// program for the next render cycle.
		void applyMaterial(std::function<void(engine::glShader*)> uniforms);
	private: 
		std::vector<engine::glTexture*> mTextures;
		engine::glShader* mShader;
	};

	// A basic renderer component.
	// Holds the mesh to the object and
	// gets called every frame.
	class bRendererComponent : public bComponent {
	public:
		bRendererComponent(core::glVertexArray vertices, core::glIndexArray indicies, engine::glMeshLayout layout, base::bEntity* camera);
		~bRendererComponent();

		bool hasRequirements() override;

		void render(float deltaTime) override;

		// Renders the entity containing the component from another camera.
		void renderFrom(float deltaTime, base::bEntity* camera);

		void setEntity(bEntity* entity) override;
	private:
		engine::glMesh* mMesh;
		base::bEntity*  mCamera;
	};

	// A basic camera component.
	// Has some built-in movement schemes.
	class bCameraComponent : public bComponent {
	public:
		bCameraComponent(float fieldOfView, float aspect, float nearPlane, float farPlane);

		bool hasRequirements() override;

		// The front vector of the camera.
		glm::vec3 getCameraFront();
		// The up vector of the camera.
		glm::vec3 getCameraUp();
		// The right vector of the camera.
		glm::vec3 getCameraRight();

		// Sets the direction of the camera.
		void setCameraDirection(glm::vec3 dir);

		// The basic projection matrix set by the constructor.
		glm::mat4x4 getProjectionMatrix();
		// The view matrix containing the position and rotation of the camera.
		glm::mat4x4 getViewMatrix();

		// Sets the aspect ratio for the base matrix,
		// for example when resizing the window.
		void setAspectRatio(float aspect);
	private:
		bTransformComponent* mTransform;

		glm::vec3 mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 mCameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

		float mFieldOfView;
		float mAspectRatio;
		float mNearPlane;
		float mFarPlane;

		glm::mat4x4 mBaseMatrix;
	};

	// A base entity class. Doesn't have any functionality
	// in itself, but accepts any bComponents that can extend
	// it's usefullness.
	class bEntity
	{
	public:
		bEntity();
		~bEntity();

		/// <summary>
		/// Adds a component to the entity's inner list.
		/// </summary>
		/// <param name="component">The component you wish to add.</param>
		void addComponent(bComponent* component);

		/// <summary>
		/// Checks if the entity has the specified type of component.
		/// </summary>
		/// <typeparam name="T">A type extended from the bComponent class.</typeparam>
		/// <returns>True if there's a component with this type on the entity.</returns>
		template<typename T>
		bool hasComponent();

		/// <summary>
		/// Gets a pointer to the specified type of component.
		/// </summary>
		/// <typeparam name="T">A type extended from the bComponent class.</typeparam>
		/// <returns>The pointer to the component. If not found, then returns nullptr.</returns>
		template<typename T>
		T* getComponent();

		/// <summary>
		/// Tries to get the component that's specified in the template,
		/// if successful than uses the callback.
		/// </summary>
		/// <typeparam name="T">A type extended from the bComponent class.</typeparam>
		template<typename T>
		void getComponent(std::function<void(T*)> callback);

		// Called every frame when rendering.
		void render(float deltaTime);
		// Called every frame after rendering.
		void update(float deltaTime);
		// Called every gametick.
		void updateFixed(float deltaTime);
	private:
		std::vector<bComponent*> mComponents;
	};

	template<typename T>
	inline bool bEntity::hasComponent()
	{
		for (bComponent* component : this->mComponents)
			if (typeid(*component) == typeid(T))
				return true;
		return false;
	}

	template<typename T>
	inline T* bEntity::getComponent()
	{
		for (bComponent* component : this->mComponents) {
			if (typeid(*component) == typeid(T))
				return (T*)component;
		}
		return nullptr;
	}

	template<typename T>
	inline void bEntity::getComponent(std::function<void(T*)> callback)
	{
		T* component = getComponent<T>();
		if (component != nullptr)
			callback(component);
	}
}