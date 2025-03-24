#include "Scene.h"

#include <cassert>

#include "Entities.h"
#include "DamageSystem.h"
#include "SpawnerSystem.h"
#include "DeathSystem.h"
#include "CooldownSystem.h"
#include "Events.h"
#include <filesystem>

Scene::Scene(Hori::ResourceHandle<YAML::Node> handle)
	: m_handle(handle)
{}

//TODO: figure out how to safely destroy (some issuaes with destruction order in world)
Scene::~Scene() = default;

Scene& Scene:: operator=(const Scene&)
{
	Clear();
	return *this;
}

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
	m_entities.push_back(entities::spawnPlayer(*playerNodePtr));

	for (const auto& pathNode : sceneNode["enemies"])
	{
		std::filesystem::path enemyNodePath{ pathNode.as<std::string>() };
		auto enemyNodePtr = resourceMng.Get<YAML::Node>(enemyNodePath);
		if (!enemyNodePtr)
		{
			std::cout << "Error: Failed to get resource " << enemyNodePath << '\n';
			return false;
		}
		m_entities.push_back(entities::spawnEnemy(*enemyNodePtr));
		m_enemyCount++;
	}
	
	std::filesystem::path nextScenePath = sceneNode["next_scene"].as<std::string>();
	m_nextSceneHandle = resourceMng.Load<YAML::Node>(nextScenePath);

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

	auto& eventMng = Hori::EventManager::GetInstance();
	eventMng.Subscribe<EnemyDeathEvent>(
		[this](const EnemyDeathEvent& e) {
			this->m_enemyCount--;
		}
	);
}

bool Scene::Reload()
{
	Clear();
	return Init();
}

bool Scene::IsComplete() const
{
	return m_enemyCount == 0;
}

Hori::ResourceHandle<YAML::Node> Scene::GetNextSceneHandle() const
{
	return m_nextSceneHandle;
}

void Scene::Clear()
{
	auto& world = Hori::Ecs::GetInstance();

	for (auto& e : world.GetEntitiesWith<ActorComponent>())
	{
		world.RemoveEntity(e);
	}

	m_entities.clear();
}
