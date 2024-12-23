#pragma once

#include <filesystem>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <Core/Texture.h>
#include <Core/ResourceManager.h>
#include <Core/VelocityComponent.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "DamageComponent.h"

namespace fs = std::filesystem;

// Not sure if this is the best way, but it's simple
struct EnemyProjectileComponent
{

};

struct PlayerProjectileComponent
{

};

struct ProjectileBlueprint
{
	ProjectileBlueprint(YAML::Node projectileData)
	{
		fs::path spritePath = projectileData["sprite"].as<std::string>();
		fs::path shader_name = projectileData["shader"].as<std::string>();
		texture = Hori::LoadTextureFromFile(spritePath, true);
		shader = Hori::LoadShaderFromFile(shader_name.replace_extension(".vs"), shader_name.replace_extension(".fs"));

		damage = DamageComponent(projectileData["damage"].as<float>());
		cooldown = projectileData["cooldown"].as<float>();
		velocity.speed = projectileData["speed"].as<float>();
		velocity.dir = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(projectileData["direction"].as<float>()));
		
		/*auto dirs = projectileData["directions"];
		directions.reserve(dirs.size());
		for (size_t i = 0; i < dirs.size(); i++)
		{
			directions.push_back(dirs[i].as<float>());
		}*/
	}

	
	float cooldown{};
	DamageComponent damage{};
	Hori::VelocityComponent velocity{};
	Hori::Texture2D texture{};
	Hori::Shader shader{};
	std::vector<float> directions{};
};