#pragma once

#include <Core/Ecs.h>
#include <Core/EventManager.h>
#include <Core/Renderer.h>
#include <Core/PrimitivesGeneration.h>
#include <Core/Components.h>

#include <yaml-cpp/yaml.h>

#include "YamlUtils.h"

inline Hori::Entity spawnEnemy(YAML::Node& data)
{
	auto& world = Hori::Ecs::GetInstance();
	auto& resourceMng = Hori::ResourceManager::GetInstance();

	std::filesystem::path spritePath = data["sprite"].as<std::string>();
	std::filesystem::path shaderPath = data["shader"].as<std::string>();
	std::filesystem::path projectilesPath = "data/guns.yaml";
	
	auto spritePtr = resourceMng.Get<Hori::SpriteComponent>(spritePath);
	auto shaderPtr = resourceMng.Get<Hori::ShaderComponent>(shaderPath);
	auto projectilesPtr = resourceMng.Get<YAML::Node>(projectilesPath);

	if (!spritePtr || !shaderPtr || !projectilesPtr)
	{
		std::cout << "Failed to load enemy resources\n";
		return {};
	}

	Hori::SpriteComponent sprite = *spritePtr;
	Hori::ShaderComponent shader = *shaderPtr;
	YAML::Node& projectiles = *projectilesPtr;
	
	YAML::Node posNode = data["position"];
	glm::vec2 position = { posNode[0].as<float>(), posNode[1].as<float>() };
	float rotation = data["rotation"].as<float>();

	Hori::TransformComponent transform = {
		.position = { data["position"][0].as<float>(), data["position"][1].as<float>() },
		.rotation = rotation,
		.scale = {data["size"][0].as<float>(), data["size"][1].as<float>()}
	};

	std::string projectileNames = data["weapon"].as<std::string>();

	std::set<Hori::Entity> prototypes = loadProjectilePackage(projectiles[projectileNames]);
	CooldownComponent cooldowns{};
	for (auto& prototype : prototypes)
	{
		cooldowns.cooldowns.emplace_back(prototype, CooldownType::ProjectileSpawn, 0.5f);
	}

	Hori::SphereCollider collider{ transform };

	glm::vec2 screenDim = Hori::Renderer::GetInstance().GetWindowSize();

	auto speed = data["speed"].as<float>();
	Hori::VelocityComponent velocity({0.f, 0.f}, speed);

	auto health = HealthComponent(data["health"].as<int>());

	std::vector<float> vertices = generateCircleVertices(0.5f, 10);
	auto wireframe = Hori::WireframeComponent(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

	Hori::Entity enemy = world.CreateEntity();
	world.AddComponents(enemy, std::move(sprite), std::move(transform), std::move(shader), std::move(velocity), std::move(health), std::move(cooldowns), std::move(collider), Hori::SpriteComponent(), EnemyComponent(), SpawnerComponent(), std::move(wireframe));

	return enemy;
}

inline Hori::Entity spawnPlayer(YAML::Node& data)
{
	auto& world = Hori::Ecs::GetInstance();
	auto& resourceMng = Hori::ResourceManager::GetInstance();

	glm::vec2 screenDim = Hori::Renderer::GetInstance().GetWindowSize();
	
	std::filesystem::path spritePath = data["sprite"].as<std::string>();
	std::filesystem::path shaderPath = data["shader"].as<std::string>();
	std::filesystem::path projectilesPath = "data/guns.yaml";
	
	auto spritePtr = resourceMng.Get<Hori::SpriteComponent>(spritePath);
	auto shaderPtr = resourceMng.Get<Hori::ShaderComponent>(shaderPath);
	auto projectilesPtr = resourceMng.Get<YAML::Node>(projectilesPath);

	if (!spritePtr || !shaderPtr || !projectilesPtr)
	{
		std::cout << "Failed to load enemy resources\n";
		return {};
	}

	Hori::SpriteComponent sprite = *spritePtr;
	Hori::ShaderComponent shader = *shaderPtr;
	YAML::Node& projectiles = *projectilesPtr;
	
	std::string projectileNames = data["weapon"].as<std::string>();
	std::set<Hori::Entity> prototypes = loadProjectilePackage(projectiles[projectileNames]);
	CooldownComponent cooldowns{};
	for (auto& prototype : prototypes)
	{
		cooldowns.cooldowns.emplace_back(prototype, CooldownType::ProjectileSpawn, 0.5f);
	}

	Hori::TransformComponent transform = {
		.position = { data["spawn"][0].as<float>(), data["spawn"][1].as<float>() },
		.rotation = 0.0f,
		.scale = {data["size"].as<float>(), data["size"].as<float>()}
	};

	float speed = data["speed"].as<float>();
	glm::vec2 direction = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(transform.rotation - 90.f));;
	Hori::VelocityComponent velocity(direction, speed);

	auto health = HealthComponent(data["health"].as<int>());
	Hori::SphereCollider collider{ transform };

	std::vector<float> vertices = generateCircleVertices(0.5f, 10);
	Hori::WireframeComponent wireframe{ vertices, glm::vec3(0.0f, 1.0f, 0.0f) };
	
	Hori::Entity player = world.CreateEntity();
	world.AddComponents(player, std::move(transform), std::move(shader), std::move(sprite), std::move(velocity), std::move(collider), std::move(cooldowns), PlayerComponent(), Hori::SpriteComponent(), Hori::ControllerComponent(), SpawnerComponent(), std::move(wireframe));

	return player;
}
