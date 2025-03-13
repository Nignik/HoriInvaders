#pragma once

#include <memory>

#include <Core/Ecs.h>
#include <Core/EventManager.h>
#include <Core/Renderer.h>
#include <Core/PrimitivesGeneration.h>
#include <Core/Components.h>

#include "HealthComponent.h"
#include "YamlUtils.h"
#include "SpawnerComponent.h"
#include "CooldownComponent.h"

struct EnemyDeathEvent : public Hori::IEvent
{
	EnemyDeathEvent(Hori::Entity deadEnemy)
		: deadEnemy(deadEnemy)
	{
	}

	Hori::Entity deadEnemy{};
};

struct EnemyComponent {};

inline Hori::Entity createEnemyPrototype(YAML::Node blueprintInfo)
{
	auto& world = Hori::Ecs::GetInstance();
	auto& resourceMng = Hori::ResourceManager::GetInstance();

	std::filesystem::path texturePath = blueprintInfo["sprite"].as<std::string>();
	std::filesystem::path shaderPath = blueprintInfo["shader"].as<std::string>();
	std::filesystem::path projectilePackagePath = "data/guns.yaml";

	auto spriteHandle = resourceMng.Load<Hori::SpriteComponent>(texturePath);
	auto shaderHandle = resourceMng.Load<Hori::ShaderComponent>(shaderPath);
	auto projectilePackageHandle = resourceMng.Load<YAML::Node>(projectilePackagePath.string());

	auto sprite = *resourceMng.Get(spriteHandle);
	auto shader = *resourceMng.Get(shaderHandle);
	auto projectilePackageNode = *resourceMng.Get(projectilePackageHandle);

	std::string projectilePackage = blueprintInfo["weapon"].as<std::string>();

	auto prototypes = loadProjectilePackage(projectilePackageNode[projectilePackage]);
	auto cooldowns = CooldownComponent();
	for (auto& prototype : prototypes)
	{
		cooldowns.cooldowns.emplace_back(prototype, CooldownType::ProjectileSpawn, 0.5f);
	}

	auto screenDim = Hori::Renderer::GetInstance().GetWindowSize();
	auto position = blueprintInfo["position"];
	auto rotation = blueprintInfo["rotation"].as<float>();
	auto size = blueprintInfo["size"].as<float>();
	Hori::TransformComponent transform = {
		.position = { position[0].as<float>(), position[0].as<float>()},
		.rotation = rotation,
		.scale = { size, size }
	};

	auto speed = blueprintInfo["speed"].as<float>();
	auto direction = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(rotation - 90.f));
	Hori::VelocityComponent velocity(direction, speed);

	auto health = HealthComponent(blueprintInfo["health"].as<int>());

	auto vertices = generateCircleVertices(0.5f, 10);
	auto wireframe = Hori::WireframeComponent(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

	auto enemy = world.CreatePrototypeEntity();
	world.AddComponents(enemy, std::move(sprite), std::move(shader), std::move(transform), std::move(velocity), std::move(health), std::move(cooldowns), Hori::SphereCollider(transform), Hori::SpriteComponent(), EnemyComponent(), SpawnerComponent(), std::move(wireframe));

	return enemy;
}