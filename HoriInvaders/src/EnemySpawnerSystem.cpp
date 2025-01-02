#include "EnemySpawnerSystem.h"

#include <World.h>
#include <Core/Sprite.h>
#include <Core/Transform.h>
#include <Core/Renderer.h>
#include <Core/Collider.h>

#include "EnemyFactoryComponent.h"
#include "ProjectileFactoryComponent.h"

EnemySpawnerSystem::EnemySpawnerSystem()
{

}

void EnemySpawnerSystem::Update(float deltaTime)
{
	auto& world = Hori::World::GetInstance();

	// For every entity that is an enemy factory check cooldown, and possibly spawn enemy
	for (const auto& factoryEntity : world.GetEntitiesWithComponents<EnemyFactoryComponent>())
	{
		auto factory = world.GetComponent<EnemyFactoryComponent>(factoryEntity);
		auto cd = world.GetComponent<CooldownComponent>(factoryEntity);
		if (cd->ready)
		{
			auto enemy = Spawn(factory->enemyPrototype);
			factory->entities.insert(enemy);
			cd->ready = false;
		}

		// Check for every enemy, whether it has left the screen, if so delete
		std::vector<Hori::Entity> removed;
		for (auto& enemy : factory->entities)
		{
			auto& position = world.GetComponent<Hori::Transform>(enemy)->position;
			const auto& screenSize = Hori::Renderer::GetInstance().GetScreenSize();
			if (position.x < 0 || position.x > screenSize.x || position.y < 0 || position.y > screenSize.y)
			{
				world.RemoveEntity(enemy);
				removed.push_back(enemy);
			}
		}

		for (auto& enemyEntity : removed)
		{
			factory->entities.erase(enemyEntity);
		}
	}
}

Hori::Entity EnemySpawnerSystem::Spawn(Hori::Entity enemyPrototype)
{
	auto& world = Hori::World::GetInstance();
	auto enemy = world.Clone(enemyPrototype);

	return enemy;
}
