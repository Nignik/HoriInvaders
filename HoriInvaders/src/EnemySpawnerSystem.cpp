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
		if (factory->cooldown <= 0.f)
		{
			auto entt = Spawn(factory->blueprint);
			factory->entities.insert(entt);
			factory->cooldown = factory->baseCooldown;
		}
		else
		{
			factory->cooldown -= deltaTime;
		}
		

		// Check for every enemy, whether it has left the screen, if so delete
		std::vector<Hori::Entity> removed;
		for (auto& enemyEntity : factory->entities)
		{
			auto& position = world.GetComponent<Hori::Transform>(enemyEntity)->position;
			const auto& screenSize = Hori::Renderer::GetInstance().GetScreenSize();
			if (position.x < 0 || position.x > screenSize.x || position.y < 0 || position.y > screenSize.y)
			{
				world.RemoveEntity(enemyEntity);
				removed.push_back(enemyEntity);
			}
		}

		for (auto& enemyEntity : removed)
		{
			factory->entities.erase(enemyEntity);
		}
	}
}

Hori::Entity EnemySpawnerSystem::Spawn(std::shared_ptr<EnemyBlueprint> enemy)
{
	auto& world = Hori::World::GetInstance();

	Hori::Transform transform = {
		.position = { 100.0f, 100.0f },
		.rotation = 180.f,
		.scale = { 25.0f, 25.0f }
	};
	Hori::SphereCollider collider(transform, false);

	const auto& entt = world.CreateEntity();
	world.AddComponent(entt, EnemyComponent());
	world.AddComponent(entt, Hori::Sprite());
	world.AddComponent(entt, transform);
	world.AddComponent(entt, collider);
	world.AddComponent(entt, enemy->velocity);
	world.AddComponent(entt, enemy->sprite);
	world.AddComponent(entt, enemy->shader);
	world.AddComponent(entt, ProjectileFactoryComponent());
	world.AddComponent(entt, enemy->gun);
	world.AddComponent(entt, enemy->health);

	return entt;
}
