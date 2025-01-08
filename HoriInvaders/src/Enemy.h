#pragma once

#include <memory>

#include <World.h>
#include <Core/Texture.h>
#include <Core/Shader.h>
#include <Core/VelocityComponent.h>
#include <Core/EventManager.h>
#include <Core/Renderer.h>

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
	std::filesystem::path spritePath = blueprintInfo["sprite"].as<std::string>();
	std::filesystem::path shaderPath = blueprintInfo["shader"].as<std::string>();
	std::filesystem::path projectilePackagePath = "data/guns.yaml";

	auto sprite = Hori::LoadTextureFromFile(spritePath, true);
	auto shader = Hori::LoadShaderFromFile(shaderPath.replace_extension(".vs"), shaderPath.replace_extension(".fs"));

	auto projectilePackageBlueprint = YAML::LoadFile(projectilePackagePath.string());
	std::string projectilePackage = blueprintInfo["weapon"].as<std::string>();

	auto prototypes = loadProjectilePackage(projectilePackageBlueprint[projectilePackage]);
	auto cooldowns = CooldownComponent();
	for (auto& prototype : prototypes)
	{
		cooldowns.cooldowns.emplace_back(prototype, CooldownType::ProjectileSpawn, 0.5f);
	}

	auto screenDim = Hori::Renderer::GetInstance().GetWindowSize();
	auto position = blueprintInfo["position"];
	auto rotation = blueprintInfo["rotation"].as<float>();
	auto size = blueprintInfo["size"].as<float>();
	Hori::Transform transform = {
		.position = { position[0].as<float>(), position[0].as<float>()},
		.rotation = rotation,
		.scale = { size, size }
	};

	auto speed = blueprintInfo["speed"].as<float>();
	auto direction = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(rotation - 90.f));
	Hori::VelocityComponent velocity(direction, speed);

	auto health = HealthComponent(blueprintInfo["health"].as<int>());

	auto& world = Hori::World::GetInstance();
	auto enemy = world.CreatePrototypeEntity();
	world.AddComponents(enemy, sprite, shader, velocity, health, transform, cooldowns, Hori::SphereCollider(transform), Hori::Sprite(), EnemyComponent(), SpawnerComponent());

	return enemy;
}