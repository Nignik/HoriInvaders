#pragma once

#include <memory>

#include <Core/Texture.h>
#include <Core/Shader.h>
#include <Core/VelocityComponent.h>
#include <Core/EventManager.h>

#include "Gun.h"
#include "HealthComponent.h"

struct EnemyDeathEvent : public Hori::IEvent
{
	EnemyDeathEvent(Hori::Entity deadEnemy)
		: deadEnemy(deadEnemy)
	{
	}

	Hori::Entity deadEnemy;
};

struct EnemyComponent
{

};

struct EnemyBlueprint
{
	EnemyBlueprint(YAML::Node blueprintInfo)
	{
		std::filesystem::path spritePath = blueprintInfo["sprite"].as<std::string>();
		std::filesystem::path shaderPath = blueprintInfo["shader"].as<std::string>();
		std::filesystem::path gunFilePath = "data/guns.yaml";

		sprite = Hori::LoadTextureFromFile(spritePath, true);
		shader = Hori::LoadShaderFromFile(shaderPath.replace_extension(".vs"), shaderPath.replace_extension(".fs"));

		auto guns = YAML::LoadFile(gunFilePath.string());
		for (const auto& key : guns)
		{
			std::cout << key.first.as<std::string>() << std::endl;
		}
		std::string gunName = blueprintInfo["weapon"].as<std::string>();
		gun = GunComponent(guns[gunName]);

		health = HealthComponent(blueprintInfo["health"].as<int>());
		velocity.speed = blueprintInfo["speed"].as<float>();
		velocity.dir = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(blueprintInfo["direction"].as<float>()));
	}

	GunComponent gun;
	Hori::Texture2D sprite{};
	Hori::Shader shader{};
	Hori::VelocityComponent velocity{};
	HealthComponent health{};
};