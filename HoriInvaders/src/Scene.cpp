#include "Scene.h"

#include <cassert>

#include "Entities.h"
#include "DamageSystem.h"
#include "SpawnerSystem.h"
#include "DeathSystem.h"
#include "CooldownSystem.h"

Scene::Scene(Hori::ResourceHandle<YAML::Node> handle)
	: m_handle(handle)
{}

bool Scene::Init()
{
	auto& resourceMng = Hori::ResourceManager::GetInstance();
	std::shared_ptr<YAML::Node> sceneNodePtr = resourceMng.Get(m_handle);
	if (!sceneNodePtr)
	{
		std::cout << "Error: Invalid scene handle\n";
		return false;
	}

	YAML::Node& sceneNode = *sceneNodePtr;

	std::filesystem::path playerNodePath{ sceneNode["player"].as<std::string>() };
	auto playerNodePtr = resourceMng.Get<YAML::Node>(playerNodePath);
	if (!playerNodePtr)
	{
		std::cout << "Error: Failed to get resource " << playerNodePath << '\n';
		return false;
	}
	m_entities.push_back(spawnPlayer(*playerNodePtr));

	for (const auto& pathNode : sceneNode["enemies"])
	{
		std::filesystem::path enemyNodePath{ pathNode.as<std::string>() };
		auto enemyNodePtr = resourceMng.Get<YAML::Node>(enemyNodePath);
		if (!enemyNodePtr)
		{
			std::cout << "Error: Failed to get resource " << enemyNodePath << '\n';
			return false;
		}
		m_entities.push_back(spawnEnemy(*enemyNodePtr));
	}

	return true;
}

// Has to be done after engine systems are initialized
void Scene::InitSystems()
{
	auto& world = Hori::Ecs::GetInstance();

	world.AddSystem<DamageSystem>(DamageSystem());
	world.AddSystem<SpawnerSystem>(SpawnerSystem());
	world.AddSystem<DeathSystem>(DeathSystem());
	world.AddSystem<CooldownSystem>(CooldownSystem());
}

bool Scene::Reload()
{
	auto& world = Hori::Ecs::GetInstance();

	for (auto& e : m_entities)
	{
		world.RemoveEntity(e);
	}

	return Init();
}
