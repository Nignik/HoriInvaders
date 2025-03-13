#include <iostream>

#include <HoriEngine.h>
#include <Core/Collider.h>
#include <Core/DebugUIComponents.h>
#include <Core/YamlInspectorComponent.h>
#include <Core/Components.h>
#include <glm/glm.hpp>

#include "Player.h"
#include "Enemy.h"
#include "DamageSystem.h"
#include "DeathSystem.h"
#include "CooldownSystem.h"
#include "SpawnerComponent.h"
#include "SpawnerSystem.h"

using namespace std;

int main()
{
	auto& engine = Hori::Engine::GetInstance();
	engine.InitSystems();
	engine.InitSingletonComponents();
	engine.InitDebugSystems();

	auto& renderer = Hori::Renderer::GetInstance();
	auto& world = Hori::Ecs::GetInstance();

	// BAD !!!!!!!!!! Damage system is not guaranteed to execute after the collision system
	world.AddSystem<DamageSystem>(DamageSystem());
	world.AddSystem<SpawnerSystem>(SpawnerSystem());
	world.AddSystem<DeathSystem>(DeathSystem());
	world.AddSystem<CooldownSystem>(CooldownSystem());

	auto& resourceMng = Hori::ResourceManager::GetInstance();
	auto enemiesHandle = resourceMng.Load<YAML::Node>("data/enemies.yaml");
	auto playerHandle = resourceMng.Load<YAML::Node>("data/player.yaml");
	auto gunsHandle = resourceMng.Load<YAML::Node>("data/guns.yaml");


	auto yamlInspector = world.CreateEntity();
	Hori::YamlInspectorComponent yamlComp;
	yamlComp.OpenFile("data/enemies.yaml");
	world.AddComponents(yamlInspector, std::move(yamlComp));

	auto fileBrowser = world.CreateEntity();
	world.AddComponents(fileBrowser, Hori::FileBrowserComponent("file browser", "C:/"));
	
	auto playerInfo = resourceMng.Get(playerHandle);
	Player player((*playerInfo)["player"]);

	auto guns = resourceMng.Get(gunsHandle);
	//world.AddComponents<GunComponent>(player.entity, GunComponent(guns["player_gun"]));

	auto enemyBlueprints = resourceMng.Get(enemiesHandle);
	auto enemyPrototype1 = createEnemyPrototype((*enemyBlueprints)["base_enemy"]);
	auto enemyPrototype2 = createEnemyPrototype((*enemyBlueprints)["other_enemy"]);
	
	auto position = glm::vec2{ 0.f, 0.f };
	float rotation = 0.0f;
	Hori::TransformComponent transform = {
		.position = position,
		.rotation = rotation,
		.scale = {4.f, 4.f}
	};

	auto enemySpawner1 = world.CreateEntity();
	world.AddComponents(enemySpawner1, std::move(transform), SpawnerComponent(), CooldownComponent({{enemyPrototype1, CooldownType::EnemySpawn, 5.f}}));
	/*for (int i = 0; i < 5; i++)
	{
		auto enemySpawner2 = world.CreateEntity();
		world.AddComponents(enemySpawner2, SpawnerComponent(), CooldownComponent({ {enemyPrototype2, CooldownType::EnemySpawn, 3.f} }));
	}*/


	engine.Run();

	return 0;
}
