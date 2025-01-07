#pragma once

#include <string>
#include <World.h>
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
			auto projectilePrototype = createProjectilePrototype(it->second);
			projectilePrototypes.emplace_back(projectilePrototype);
		}
	}

	std::vector<Hori::Entity> projectilePrototypes{};
};