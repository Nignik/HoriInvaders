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

using namespace std;

int main()
{
	auto& engine = Hori::Engine::GetInstance();
	engine.InitSystems();

	auto& renderer = Hori::Renderer::GetInstance();
	auto& world = Hori::World::GetInstance();

	world.AddSystem<ShootingSystem>(ShootingSystem());
	
	const auto& player = world.CreateEntity();
	world.AddComponent(player, Hori::Sprite());
	world.AddComponent(player, Hori::Controller());
	world.AddComponent(player, Hori::VelocityComponent({0.0f, 0.0f}, 500.0f));
	world.AddComponent(player, Hori::Transform({ 200.0f, 200.0f }, 0.0f, { 100.0f, 100.0f }));
	world.AddComponent(player, Hori::SphereCollider(Hori::Transform({ 300.0f, 300.0f }, 0.0f, { 50.0f, 50.0f }), 100.0f));
	world.AddComponent(player, Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs"));
	world.AddComponent(player, Hori::LoadTextureFromFile("resources/textures/awesomeface.png", true));
	
	auto guns = YAML::LoadFile("data/guns.yaml");
	const auto& enemy = world.CreateEntity();
	world.AddComponent(enemy, Hori::Sprite());
	world.AddComponent(enemy, Gun(guns["base_gun"]));
	world.AddComponent(enemy, ProjectileFactory());
	world.AddComponent(enemy, Hori::Transform({ 100.0f, 100.0f }, 0.0f, { 50.0f, 50.0f }));
	world.AddComponent(enemy, Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs"));
	world.AddComponent(enemy, Hori::LoadTextureFromFile("resources/textures/awesomeface.png", true));

	engine.Run();

	return 0;
}
