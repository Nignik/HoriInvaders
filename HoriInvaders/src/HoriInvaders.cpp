#include <iostream>

#include <HoriEngine.h>
#include <Core/Sprite.h>
#include <Core/Controller.h>
#include <Core/Collider.h>
#include <Core/Shader.h>
#include <Core/Texture.h>
#include <Core/TextComponent.h>
#include <Core/DebugUIComponents.h>
#include <Core/YamlInspectorComponent.h>
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

	auto yamlInspector = world.CreateEntity();
	Hori::YamlInspectorComponent yamlComp;
	yamlComp.OpenFile("data/enemies.yaml");
	world.AddComponents(yamlInspector, yamlComp);

	auto fileBrowser = world.CreateEntity();
	world.AddComponents(fileBrowser, Hori::FileBrowserComponent("file browser", "C:/"));

	auto playerInfo = YAML::LoadFile("data/player.yaml");
	Player player(playerInfo["player"]);

	auto guns = YAML::LoadFile("data/guns.yaml");
	//world.AddComponents<GunComponent>(player.entity, GunComponent(guns["player_gun"]));

	auto enemyBlueprints = YAML::LoadFile("data/enemies.yaml");
	auto enemyPrototype1 = createEnemyPrototype(enemyBlueprints["base_enemy"]);
	auto enemyPrototype2 = createEnemyPrototype(enemyBlueprints["other_enemy"]);

	auto enemySpawner1 = world.CreateEntity();
	world.AddComponents(enemySpawner1, SpawnerComponent(), CooldownComponent({{enemyPrototype1, CooldownType::EnemySpawn, 3.f}}));
	/*for (int i = 0; i < 5; i++)
	{
		auto enemySpawner2 = world.CreateEntity();
		world.AddComponents(enemySpawner2, SpawnerComponent(), CooldownComponent({ {enemyPrototype2, CooldownType::EnemySpawn, 3.f} }));
	}*/


	engine.Run();

	return 0;
}
