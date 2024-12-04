#pragma once

#include <vector>
#include <yaml-cpp/yaml.h>
#include <Core/Texture.h>
#include <Core/ResourceManager.h>
#include <Core/VelocityComponent.h>

struct Projectile
{
	Projectile(YAML::Node projectileData)
	{
		damage = projectileData["damage"].as<float>();
		cooldown = projectileData["cooldown"].as<float>();
		velocity.velocity.x = projectileData["velocity"][0].as<float>();
		velocity.velocity.y = projectileData["velocity"][1].as<float>();
		texture = Hori::LoadTextureFromFile(projectileData["sprite"].as<std::string>(), true);
		auto dirs = projectileData["directions"];
		directions.reserve(dirs.size());
		for (size_t i = 0; i < dirs.size(); i++)
		{
			directions.push_back(dirs[i].as<float>());
		}
	}

	float damage{};
	float cooldown{};
	Hori::VelocityComponent velocity{};
	Hori::Texture2D texture{};
	std::vector<float> directions{};
};