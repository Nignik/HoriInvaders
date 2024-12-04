#pragma once

#include <string>

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

#include "Projectile.h"

/*
A gun spawns projectiles. Projectiles are new entities.
*/

struct Gun
{
	Gun(YAML::Node& gunData)
	{
		auto projs = gunData["projectiles"];
		projectiles.reserve(projectiles.size());
		for (size_t i = 0; i < projs.size(); i++)
		{
			projectiles.emplace_back(projs[i]);
		}
	}

	std::vector<Projectile> projectiles{};
};