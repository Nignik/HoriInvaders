#pragma once

#include <vector>
#include <yaml-cpp/yaml.h>
#include <Core/Texture.h>
#include <Core/ResourceManager.h>
#include <Core/VelocityComponent.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

struct Projectile
{
	Projectile(YAML::Node projectileData)
	{
		damage = projectileData["damage"].as<float>();
		cooldown = projectileData["cooldown"].as<float>();
		velocity.speed = projectileData["speed"].as<float>();
		velocity.dir = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(projectileData["direction"].as<float>()));
		texture = Hori::LoadTextureFromFile(projectileData["sprite"].as<std::string>(), true);

		const auto& shader_name = projectileData["shader"].as<std::string>();
		shader = Hori::LoadShaderFromFile(shader_name + ".vs", shader_name + ".fs");
		/*auto dirs = projectileData["directions"];
		directions.reserve(dirs.size());
		for (size_t i = 0; i < dirs.size(); i++)
		{
			directions.push_back(dirs[i].as<float>());
		}*/
	}

	float damage{};
	float cooldown{};
	Hori::VelocityComponent velocity{};
	Hori::Texture2D texture{};
	Hori::Shader shader{};
	std::vector<float> directions{};
};