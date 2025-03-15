#pragma once

#include <World.h>
#include <set>

struct EnemyProjectileComponent {};
struct PlayerProjectileComponent {};
struct EnemyComponent {};
struct PlayerComponent {};

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

struct DamageComponent
{
	float value = 0.f;
};

struct HealthComponent
{
	int value;
};

/*
* Spawner component only stores the spawned entities and makes the entity visible to spawner system
* Projectile prototypes are stored in the cooldown component
*/
struct SpawnerComponent
{
	std::set<Hori::Entity> spawned{};
};