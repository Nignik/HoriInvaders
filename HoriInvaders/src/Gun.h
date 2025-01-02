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
			auto projectilePrototype = createProjectilePrototype(it->second);
			projectilePrototypes.emplace_back(projectilePrototype);

			// This needs to be changed, the entities dont ever get deleted, and its ugly
			auto& world =Hori::World::GetInstance();
			auto cooldown = world.CreateEntity();
			world.AddComponents(cooldown, CooldownComponent(0.5f));
			reaload[projectilePrototype] = cooldown;
		}
	}

	std::vector<Hori::Entity> projectilePrototypes{};
	std::unordered_map<Hori::Entity, Hori::Entity> reaload{};
};