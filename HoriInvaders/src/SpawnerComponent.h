#pragma once

#include <vector>
#include <set>
#include <Entity.h>
#include <yaml-cpp/yaml.h>

/*
* Spawner component only stores the spawned entities and makes the entity visible to spawner system
* Projectile prototypes are stored in the cooldown component
*/
struct SpawnerComponent
{
	std::set<Hori::Entity> spawned{};
};