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
	EnemyFactoryComponent(Hori::Entity enemyPrototype, float baseCooldown = 5.f, float cooldown = 0.f)
		: enemyPrototype(enemyPrototype)
	{

	}

	Hori::Entity enemyPrototype;
	std::set<Hori::Entity> entities{};
};