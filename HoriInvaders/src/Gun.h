#pragma once

#include <string>

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

#include "ProjectileBlueprint.h"

/*
A gun spawns projectiles. Projectiles are new entities.
*/

struct Gun
{
	Gun() = default;

	Gun(YAML::Node gunData)
	{
		auto projs = gunData["projectiles"];
		projectiles.reserve(projectiles.size());
		for (auto it = projs.begin(); it != projs.end(); ++it)
		{
			projectiles.emplace_back(it->second);
		}

		reload.resize(projectiles.size());
	}

	std::vector<ProjectileBlueprint> projectiles{};
	std::vector<float> reload{};
};