#pragma once

#include "includes.hpp"
#include "framebuffer.hpp"
#include "entity.hpp"

#include <vector>

namespace base {
	/// <summary>
	/// The main manager of the game engine. It uses a singleton type implementation.
	/// </summary>
	class bEngineManager
	{
	public:
		/// <summary>
		/// Inits the singelton instance.
		/// </summary>
		static void Init();
		/// <summary>
		/// Destroys the singleton.
		/// </summary>
		static void Destroy();

		/// <summary>
		/// The main instance of the manager. 
		/// </summary>
		inline static bEngineManager* Instance;

		bEngineManager();
		~bEngineManager();

		/// <summary>
		/// The render function that calls all the renderable objects by default.
		/// </summary>
		/// <param name="deltaTime">: The time since the last frame.</param>
		void Render(float deltaTime);
		/// <summary>
		/// Renders the scene from a specified camera.
		/// </summary>
		/// <param name="deltaTime">: The time since the last frame.</param>
		/// <param name="camera">: The specified camera.</param>
		void RenderFrom(float deltaTime, base::bEntity* camera);

		/// <summary>
		/// Registers a framebuffer in the list.
		/// </summary>
		/// <param name="buffer">: The framebuffer to register.</param>
		void RegisterFramebuffer(engine::glFramebuffer* buffer);
		/// <summary>
		/// Registers a renderable object in the list.
		/// </summary>
		/// <param name="entity">: The entity to register.</param>
		void RegisterRenderable(base::bEntity* entity);
	private:
		std::vector<engine::glFramebuffer*>* mFramebufferList;
		std::vector<base::bEntity*>*			  mEntityList;
	};
}