#include <iostream>

#include <HoriEngine.h>
#include <Core/Sprite.h>
#include <Core/Controller.h>
#include <Core/Collider.h>
#include <Core/Shader.h>
#include <Core/Texture.h>

#include <glm/glm.hpp>

#include "Gun.h"
#include "ProjectileSpawnerSystem.h"
#include "ProjectileFactoryComponent.h"
#include "Player.h"
#include "Enemy.h"
#include "DamageSystem.h"
#include "EnemyFactoryComponent.h"
#include "EnemySpawnerSystem.h"
#include "DeathSystem.h"
#include "CooldownSystem.h"

using namespace std;

int main()
{
	auto& engine = Hori::Engine::GetInstance();
	engine.InitSystems();

	auto& renderer = Hori::Renderer::GetInstance();
	auto& world = Hori::World::GetInstance();

	// BAD !!!!!!!!!! Damage system is not guaranteed to execute after the collision system
	world.AddSystem<DamageSystem>(DamageSystem());
	world.AddSystem<ProjectileSpawnerSystem>(ProjectileSpawnerSystem());
	world.AddSystem<EnemySpawnerSystem>(EnemySpawnerSystem());
	world.AddSystem<DeathSystem>(DeathSystem());
	world.AddSystem<CooldownSystem>(CooldownSystem());

	auto playerInfo = YAML::LoadFile("data/player.yaml");
	Player player(playerInfo["player"]);

	auto guns = YAML::LoadFile("data/guns.yaml");
	world.AddComponents<GunComponent>(player.entity, GunComponent(guns["player_gun"]));

	auto enemyBlueprints = YAML::LoadFile("data/enemies.yaml");
	auto enemyPrototype = createEnemyPrototype(enemyBlueprints["base_enemy"]);

	auto enemySpawner = world.CreateEntity();
	world.AddComponents(enemySpawner, EnemyFactoryComponent(enemyPrototype), CooldownComponent(5.f));

	engine.Run();

	return 0;
}
