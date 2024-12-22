#pragma once

#include <vector>
#include <set>
#include <Entity.h>

// Keeps track of projectiles shot by the gun, so that they can later be deleted
struct ProjectileFactoryComponent
{
	std::set<Hori::Entity> projectileEntities{};
};