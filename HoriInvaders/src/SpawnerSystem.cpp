#include "SpawnerSystem.h"
#include "SpawnerComponent.h"
#include "Enemy.h"
#include "Player.h"
#include "CooldownComponent.h"

#include <Core/Ecs.h>
#include <Core/Sprite.h>
#include <Core/Transform.h>
#include <Core/Renderer.h>
#include <Core/Collider.h>
#include <cmath>

SpawnerSystem::SpawnerSystem()
{

}

void SpawnerSystem::Update(float deltaTime)
{
	auto& world = Hori::Ecs::GetInstance();

	for (auto spawnerEntity : world.GetEntitiesWith<SpawnerComponent>())
	{
		auto& cooldowns = world.GetComponent<CooldownComponent>(spawnerEntity)->cooldowns;

		for (auto& cooldown : cooldowns)
		{
			if (!cooldown.ready)
				continue;

			switch (cooldown.type)
			{
				case CooldownType::ProjectileSpawn:		SpawnProjectile(cooldown.entity, spawnerEntity);		break;
				case CooldownType::EnemySpawn:			SpawnEnemy(cooldown.entity, spawnerEntity);				break;
			}

			cooldown.ready = false;
		}
	}
}

Hori::Entity SpawnerSystem::SpawnProjectile(Hori::Entity& prototype, Hori::Entity& spawnerEntity)
{
	auto& world = Hori::Ecs::GetInstance();

	auto spawnerComponent = world.GetComponent<SpawnerComponent>(spawnerEntity);
	auto transform = world.GetComponent<Hori::Transform>(spawnerEntity);

	auto projectile = world.Clone(prototype);
	world.GetComponent<Hori::Transform>(projectile)->position = transform->position;
	world.GetComponent<Hori::SphereCollider>(projectile)->transform = Hori::SphereCollider(*transform, true).transform;

	if (world.HasComponents<EnemyComponent>(spawnerEntity))
		world.AddComponents(projectile, EnemyProjectileComponent());
	else if (world.HasComponents<PlayerComponent>(spawnerEntity))
		world.AddComponents(projectile, PlayerProjectileComponent());

	spawnerComponent->spawned.insert(projectile);

	return projectile;
}

Hori::Entity SpawnerSystem::SpawnEnemy(Hori::Entity& prototype, Hori::Entity& spawnerEntity)
{
	auto& world = Hori::Ecs::GetInstance();

	auto enemy = world.Clone(prototype);

	auto spawnerComponent = world.GetComponent<SpawnerComponent>(spawnerEntity);
	spawnerComponent->spawned.insert(enemy);

	return enemy;
}
