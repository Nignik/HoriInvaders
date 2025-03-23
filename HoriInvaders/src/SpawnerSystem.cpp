#include "SpawnerSystem.h"

#include <Core/Components.h>
#include <Core/Renderer.h>
#include <Core/Collider.h>
#include <cmath>

#include "Components.h"
#include "Entities.h"

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
			}

			cooldown.ready = false;
		}
	}
}

Hori::Entity SpawnerSystem::SpawnProjectile(Hori::Entity& prototype, Hori::Entity& spawnerEntity)
{
	auto& world = Hori::Ecs::GetInstance();

	auto spawnerComponent = world.GetComponent<SpawnerComponent>(spawnerEntity);
	auto transform = world.GetComponent<Hori::TransformComponent>(spawnerEntity);

	auto projectile = world.Clone(prototype);
	world.GetComponent<Hori::TransformComponent>(projectile)->position = transform->position;
	*world.GetComponent<Hori::SphereCollider>(projectile) = Hori::SphereCollider(*transform, true);

	if (world.HasComponents<EnemyComponent>(spawnerEntity))
		world.AddComponents(projectile, EnemyProjectileComponent());
	else if (world.HasComponents<PlayerComponent>(spawnerEntity))
		world.AddComponents(projectile, PlayerProjectileComponent());

	spawnerComponent->spawned.insert(projectile);

	return projectile;
}