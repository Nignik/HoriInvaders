#pragma once

#include <vector>
#include <set>
#include <Entity.h>

#include "EnemyBlueprint.h"

/*
*	Stores enemy entities that it spawned
*	Has cooldown which tells it when to spawn a new enemy
*/
struct EnemyFactoryComponent
{
	EnemyFactoryComponent(std::shared_ptr<EnemyBlueprint> blueprint, float baseCooldown = 5.f, float cooldown = 0.f)
		: blueprint(blueprint)
	{

	}

	std::shared_ptr<EnemyBlueprint> blueprint;
	float baseCooldown = 5.f;
	float cooldown = 0.f;
	std::set<Hori::Entity> entities{};
};