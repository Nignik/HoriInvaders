#include <iostream>

#include <HoriEngine.h>
#include <Core/Sprite.h>
#include <Core/Controller.h>
#include <Core/Collider.h>
#include <Core/Shader.h>
#include <Core/Texture.h>

#include <glm/glm.hpp>

#include "Gun.h"
#include "ShootingSystem.h"
#include "ProjectileFactory.h"
#include "Player.h"
#include "DamageSystem.h"

using namespace std;

int main()
{
	auto& engine = Hori::Engine::GetInstance();
	engine.InitSystems();

	auto& renderer = Hori::Renderer::GetInstance();
	auto& world = Hori::World::GetInstance();

	// BAD !!!!!!!!!! Damage system is not guaranteed to execute after the collision system
	world.AddSystem<DamageSystem>(DamageSystem());
	world.AddSystem<ShootingSystem>(ShootingSystem());

	Hori::Transform playerTransform = {
		.position = {400.0f, 400.0f},
		.rotation = 0.0f,
		.scale = {100.0f, 100.0f}
	};
	Player player(playerTransform, 500.0f, 50);

	Hori::Transform enemyTransform = {
		.position = {100.0f, 100.0f},
		.rotation = 0.0f,
		.scale = {50.0f, 50.0f}
	};
	auto guns = YAML::LoadFile("data/guns.yaml");
	const auto& enemy = world.CreateEntity();
	world.AddComponent(enemy, Hori::Sprite());
	world.AddComponent(enemy, Gun(guns["base_gun"]));
	world.AddComponent(enemy, ProjectileFactory());
	world.AddComponent(enemy, enemyTransform);
	world.AddComponent(enemy, Hori::SphereCollider(enemyTransform, false));
	world.AddComponent(enemy, Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs"));
	world.AddComponent(enemy, Hori::LoadTextureFromFile("resources/textures/awesomeface.png", true));

	engine.Run();

	return 0;
}
