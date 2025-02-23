#pragma once

#include <yaml-cpp/yaml.h>
#include <Core/Ecs.h>
#include <set>

#include "Projectile.h"

inline std::set<Hori::Entity> loadProjectilePackage(YAML::Node blueprint)
{
	std::set<Hori::Entity> prototypes;
	auto projectiles = blueprint["projectiles"];
	// TODO: change to range loop
	for (auto it = projectiles.begin(); it != projectiles.end(); ++it)
	{
		auto prototype = createProjectilePrototype(it->second);
		prototypes.insert(prototype);
	}

	return prototypes;
}