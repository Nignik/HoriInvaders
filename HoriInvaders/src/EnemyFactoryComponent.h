#pragma once

#include <vector>
#include <set>
#include <Entity.h>

#include "Enemy.h"

/*
*	Stores enemy entities that it spawned
*	Has cooldown which tells it when to spawn a new enemy
*/
struct EnemyFactoryComponent
{
	std::set<Hori::Entity> entities{};
};