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

	std::filesystem::path texturePath = data["sprite"].as<std::string>();
	std::filesystem::path shaderPath = data["shader"].as<std::string>();
	std::filesystem::path projectilePackagePath = "data/guns.yaml";

	auto spriteHandle = resourceMng.Load<Hori::SpriteComponent>(texturePath);
	auto shaderHandle = resourceMng.Load<Hori::ShaderComponent>(shaderPath);
	auto projectilePackageHandle = resourceMng.Load<YAML::Node>(projectilePackagePath.string());

	auto sprite = *resourceMng.Get(spriteHandle);
	auto shader = *resourceMng.Get(shaderHandle);
	auto projectilePackageNode = *resourceMng.Get(projectilePackageHandle);
	
	YAML::Node posNode = data["position"];
	glm::vec2 position = { posNode[0].as<float>(), posNode[1].as<float>() };
	float rotation = data["rotation"].as<float>();


	Hori::TransformComponent transform = {
		.position = { data["position"][0].as<float>(), data["position"][1].as<float>() },
		.rotation = rotation,
		.scale = {data["size"][0].as<float>(), data["size"][1].as<float>()}
	};


	std::string projectilePackage = data["weapon"].as<std::string>();

	auto prototypes = loadProjectilePackage(projectilePackageNode[projectilePackage]);
	auto cooldowns = CooldownComponent();
	for (auto& prototype : prototypes)
	{
		cooldowns.cooldowns.emplace_back(prototype, CooldownType::ProjectileSpawn, 0.5f);
	}

	Hori::SphereCollider collider{ transform };

	auto screenDim = Hori::Renderer::GetInstance().GetWindowSize();

	auto speed = data["speed"].as<float>();
	Hori::VelocityComponent velocity({0.f, 0.f}, speed);

	auto health = HealthComponent(data["health"].as<int>());

	auto vertices = generateCircleVertices(0.5f, 10);
	auto wireframe = Hori::WireframeComponent(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

	auto enemy = world.CreateEntity();
	world.AddComponents(enemy, std::move(sprite), std::move(transform), std::move(shader), std::move(velocity), std::move(health), std::move(cooldowns), std::move(collider), Hori::SpriteComponent(), EnemyComponent(), SpawnerComponent(), std::move(wireframe));

	return enemy;
}

inline Hori::Entity spawnPlayer(YAML::Node& data)
{
	auto& world = Hori::Ecs::GetInstance();
	auto& resourceMng = Hori::ResourceManager::GetInstance();

	auto screenDim = Hori::Renderer::GetInstance().GetWindowSize();

	auto position = glm::vec2{ data["spawn"][0].as<float>(), data["spawn"][1].as<float>() };
	float rotation = 0.0f;
	Hori::TransformComponent transform = {
		.position = position,
		.rotation = rotation,
		.scale = {data["size"].as<float>(), data["size"].as<float>()}
	};

	fs::path shaderPath = data["shader"].as<std::string>();
	fs::path texturePath = data["sprite"].as<std::string>();
	
	auto spriteHandle = resourceMng.Load<Hori::SpriteComponent>(texturePath);
	auto shaderHandle = resourceMng.Load<Hori::ShaderComponent>(shaderPath);

	auto sprite = *resourceMng.Get(spriteHandle);
	auto shader = *resourceMng.Get(shaderHandle);

	float speed = data["speed"].as<float>();
	glm::vec2 direction = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(rotation - 90.f));;
	Hori::VelocityComponent velocity(direction, speed);

	auto health = HealthComponent(data["health"].as<int>());
	auto collider = Hori::SphereCollider(transform);

	auto vertices = generateCircleVertices(0.5f, 10);
	auto wireframe = Hori::WireframeComponent(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
	
	auto player = world.CreateEntity();
	world.AddComponents(player, std::move(transform), std::move(shader), std::move(sprite), std::move(velocity), std::move(collider), PlayerComponent(), Hori::SpriteComponent(), Hori::ControllerComponent(), std::move(wireframe));

	return player;
}
