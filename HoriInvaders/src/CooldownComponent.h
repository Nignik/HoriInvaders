#pragma once

#include <World.h>
#include <unordered_map>
#include <iostream>

enum class CooldownType
{
	ProjectileSpawn,
	EnemySpawn
};

// Be careful for potential memory leaks, nothing deletes the entity here
struct CooldownSlot
{
	Hori::Entity entity{};
	CooldownType type{};
	float total = 0.f;
	bool ready = true;
	float current = 0.f;
};

/*
* Component used to track the cooldown.
* Ready has to be set to false manually, but sets to true automatically
*/
struct CooldownComponent
{
	std::vector<CooldownSlot> cooldowns{};
};