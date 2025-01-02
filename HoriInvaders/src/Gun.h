#pragma once

#include <string>

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

#include "Projectile.h"

/*
A gun spawns projectiles. Projectiles are new entities.
*/

struct GunComponent
{
	GunComponent() = default;

	GunComponent(YAML::Node gunData)
	{
		auto projs = gunData["projectiles"];
		projectilePrototypes.reserve(projectilePrototypes.size());
		for (auto it = projs.begin(); it != projs.end(); ++it)
		{
			projectilePrototypes.emplace_back(createProjectilePrototype(it->second));
		}

		reload.resize(projectilePrototypes.size());
	}

	std::vector<Hori::Entity> projectilePrototypes{};
	std::vector<float> reload{};
};