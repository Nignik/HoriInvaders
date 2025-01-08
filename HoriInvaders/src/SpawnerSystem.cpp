#include "SpawnerSystem.h"
#include "SpawnerComponent.h"
#include "Enemy.h"
#include "Player.h"
#include "CooldownComponent.h"

#include <World.h>
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
	auto& world = Hori::World::GetInstance();

	for (auto spawnerEntity : world.GetEntitiesWithComponents<SpawnerComponent>())
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

		/*std::vector<Hori::Entity> removed;
		for (auto& projEntity : spawnerComponent->spawned)
		{
			auto& position = world.GetComponent<Hori::Transform>(projEntity)->position;
			auto screenSize = Hori::Renderer::GetInstance().GetCameraSize();
			if (position.x < -screenSize.x || position.x > screenSize.x || position.y < -screenSize.y || position.y > screenSize.y)
			{
				world.RemoveEntity(projEntity);
				removed.push_back(projEntity);
			}
		}

		for (auto& projEntity : removed)
		{
			spawnerComponent->spawned.erase(projEntity);
		}*/
	}
}

Hori::Entity SpawnerSystem::SpawnProjectile(Hori::Entity& prototype, Hori::Entity& spawnerEntity)
{
	auto& world = Hori::World::GetInstance();

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
	auto& world = Hori::World::GetInstance();

	auto enemy = world.Clone(prototype);

	auto spawnerComponent = world.GetComponent<SpawnerComponent>(spawnerEntity);
	spawnerComponent->spawned.insert(enemy);

	return enemy;
}
