#pragma once

#include <Core/Ecs.h>
#include <Core/EventManager.h>
#include <Core/Renderer.h>
#include <Core/PrimitivesGeneration.h>
#include <Core/Components.h>
#include <Core/ResourceManager.h>
#include <Core/Collider.h>

#include <yaml-cpp/yaml.h>

#include "ComponentParser.h"

/*struct Enemy
{
	Hori::ResourceHandle<Hori::SpriteComponent> sprite{};
	Hori::ResourceHandle<Hori::ShaderComponent> shader{};
	Hori::TransformComponent transform{};
	Hori::VelocityComponent velocity{};

	CooldownComponent cooldowns{};
	HealthComponent health{};

	std::optional<Hori::WireframeComponent> wireframe{};
};*/

inline Hori::Entity initializeCharacter(Hori::Entity character, YAML::Node& data)
{
	auto& world = Hori::Ecs::GetInstance();
	auto& resourceMng = Hori::ResourceManager::GetInstance();

	std::filesystem::path spritePath = data["sprite"].as<std::string>();
	std::filesystem::path shaderPath = data["shader"].as<std::string>();
	
	auto spritePtr = resourceMng.Get<Hori::SpriteComponent>(spritePath);
	auto shaderPtr = resourceMng.Get<Hori::ShaderComponent>(shaderPath);

	if (!spritePtr || !shaderPtr)
	{
		std::cout << "Failed to load enemy resources\n";
		return {};
	}

	Hori::SpriteComponent sprite = *spritePtr;
	Hori::ShaderComponent shader = *shaderPtr;
	
	HealthComponent health{ 100 };
	if (data["health"])
		health.value = data["health"].as<int>();

	auto transform = parse<Hori::TransformComponent>(data["transform"]);
	auto velocity = parse<Hori::VelocityComponent>(data["velocity"]);
	Hori::SphereCollider collider{ transform };

	std::vector<float> vertices = generateCircleVertices(0.5f, 10);
	auto wireframe = Hori::WireframeComponent(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

	world.AddComponents(character, std::move(sprite), std::move(transform), std::move(shader), std::move(velocity), std::move(health), std::move(collider), std::move(wireframe), ActorComponent(), Hori::SpriteComponent());
	
	return character;
}

inline Hori::Entity createProjectilePrototype(YAML::Node& data)
{
	auto& world = Hori::Ecs::GetInstance();
	Hori::Entity character = world.CreatePrototypeEntity();
	initializeCharacter(character, data);

	return character;
}

inline std::set<Hori::Entity> loadProjectilePackage(YAML::Node data)
{
	std::set<Hori::Entity> prototypes;
	for (int i = 0; i < data.size(); i++)
	{
		std::filesystem::path path = data[i].as<std::string>();
		auto projectile = Hori::ResourceManager::GetInstance().Get<YAML::Node>(path);
		if (!projectile)
			return {};

		auto prototype = createProjectilePrototype(*projectile);
		prototypes.insert(prototype);
	}

	return prototypes;
}


inline void addProjectiles(Hori::Entity e, const YAML::Node& data)
{
	std::set<Hori::Entity> prototypes = loadProjectilePackage(data);
	CooldownComponent cooldowns{};
	for (auto& prototype : prototypes)
	{
		cooldowns.cooldowns.emplace_back(prototype, CooldownType::ProjectileSpawn, 0.5f);
	}
	
	Hori::Ecs::GetInstance().AddComponents(e, std::move(cooldowns));
}

inline Hori::Entity spawnEnemy(YAML::Node& data)
{
	Hori::Entity character = Hori::Ecs::GetInstance().CreateEntity();
	initializeCharacter(character, data);
	Hori::Ecs::GetInstance().AddComponents(character, EnemyComponent(), SpawnerComponent());
	addProjectiles(character, data["projectiles"]);

	return character;
}

inline Hori::Entity spawnPlayer(YAML::Node& data)
{
	Hori::Entity character = Hori::Ecs::GetInstance().CreateEntity();
	initializeCharacter(character, data);
	Hori::Ecs::GetInstance().AddComponents(character, PlayerComponent(), Hori::ControllerComponent(), SpawnerComponent());
	addProjectiles(character, data["projectiles"]);

	return character;
}
