#include "engineManager.hpp"

namespace base {
	void bEngineManager::Init()
	{
		if (!bEngineManager::Instance)
			bEngineManager::Instance = new bEngineManager();
		else
			DebugConsole::warn("Tried to re-init the singelton engine manager. Check for init calls!");
	}

	void bEngineManager::Destroy()
	{
		if (bEngineManager::Instance)
			delete bEngineManager::Instance;
		else
			DebugConsole::warn("Tried to destroy the non-exsistent engine manager. Check for destroy calls!");
	}

	bEngineManager::bEngineManager()
	{
		mFramebufferList = new std::vector<engine::glFramebuffer*>();		
		mEntityList = new std::vector<base::bEntity*>();
	}

	bEngineManager::~bEngineManager()
	{
		delete bEngineManager::mFramebufferList;
		delete bEngineManager::mEntityList;
	}

	void bEngineManager::Render(float deltaTime)
	{
		for (base::bEntity* entity : *mEntityList) {
			entity->render(deltaTime);
		}
	}

	void bEngineManager::RenderFrom(float deltaTime, base::bEntity* camera)
	{
		for (base::bEntity* entity : *mEntityList) {
			entity->getComponent<bRendererComponent>()->renderFrom(deltaTime, camera);
		}
	}

	void bEngineManager::RegisterFramebuffer(engine::glFramebuffer* buffer)
	{
		mFramebufferList->push_back(buffer);
	}

	void bEngineManager::RegisterRenderable(base::bEntity* entity)
	{
		mEntityList->push_back(entity);
	}
}
