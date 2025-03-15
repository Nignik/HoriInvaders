#include <iostream>

#include <HoriEngine.h>
#include <Core/Collider.h>
#include <Core/DebugUIComponents.h>
#include <Core/YamlInspectorComponent.h>
#include <Core/Components.h>
#include <glm/glm.hpp>

#include "Components.h"
#include "Entities.h"
#include "DamageSystem.h"
#include "DeathSystem.h"
#include "CooldownSystem.h"
#include "SpawnerSystem.h"
#include "EntityPrototypes.h"
#include "Events.h"

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

	auto enemyHandle = resourceMng.Load<YAML::Node>("data/enemies/base_enemy.yaml");
	auto playerHandle = resourceMng.Load<YAML::Node>("data/player.yaml");
	auto gunsHandle = resourceMng.Load<YAML::Node>("data/guns.yaml");

	auto yamlInspector = world.CreateEntity();
	Hori::YamlInspectorComponent yamlComp;
	yamlComp.Open("data/enemies/base_enemy.yaml");
	world.AddComponents(yamlInspector, std::move(yamlComp));

	auto fileBrowser = world.CreateEntity();
	world.AddComponents(fileBrowser, Hori::FileBrowserComponent("file browser", "C:/"));
	
	auto playerData = resourceMng.Get(playerHandle);
	auto player = spawnPlayer(*playerData);

	auto guns = resourceMng.Get(gunsHandle);
	//world.AddComponents<GunComponent>(player.entity, GunComponent(guns["player_gun"]));

	auto enemyData = *resourceMng.Get(enemyHandle);
	auto enemy = spawnEnemy(enemyData);

	engine.Run();

	return 0;
}
