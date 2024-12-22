#include "ProjectileSpawnerSystem.h"
#include "Gun.h"
#include "ProjectileFactoryComponent.h"
#include "LayerComponent.h"

#include <World.h>
#include <Core/Sprite.h>
#include <Core/Transform.h>
#include <Core/Renderer.h>
#include <Core/Collider.h>

ProjectileSpawnerSystem::ProjectileSpawnerSystem()
{

}

void ProjectileSpawnerSystem::Update(float deltaTime)
{
	auto& world = Hori::World::GetInstance();

	// For every entity that has a gun component shoot all projectiles
	for (const auto& entity : world.GetEntitiesWithComponents<Gun>())
	{
		auto gun = world.GetComponent<Gun>(entity);
		auto factory = world.GetComponent<ProjectileFactoryComponent>(entity);
		for (int i = 0; i < gun->projectiles.size(); i++)
		{
			auto& projectile = gun->projectiles[i];
			gun->reload[i] -= deltaTime;
			if (gun->reload[i] > 0)
				continue;
			
			auto projEntity = Spawn(projectile);
			gun->reload[i] = projectile.cooldown;
			
			factory->projectileEntities.insert(projEntity);
		}

		// Check for every projectile, whether it has left the screen, if so delete
		std::vector<Hori::Entity> removed;
		for (auto& projEntity : factory->projectileEntities)
		{
			auto& position = world.GetComponent<Hori::Transform>(projEntity)->position;
			const auto& screenSize = Hori::Renderer::GetInstance().GetScreenSize();
			if (position.x < 0 || position.x > screenSize.x || position.y < 0 || position.y > screenSize.y)
			{
				world.RemoveEntity(projEntity);
				removed.push_back(projEntity);
			}
		}

		for (auto& projEntity : removed)
		{
			factory->projectileEntities.erase(projEntity);
		}
	}
}

// Creates entity in the world
Hori::Entity ProjectileSpawnerSystem::Spawn(ProjectileBlueprint& projectile)
{
	auto& world = Hori::World::GetInstance();

	Hori::Transform transform = {
		.position = { 300.0f, 300.0f },
		.rotation = 0.f,
		.scale = { 25.0f, 25.0f }
	};
	Hori::SphereCollider collider(transform, true);

	const auto& projEntity = world.CreateEntity();
	world.AddComponent(projEntity, Hori::Sprite());
	world.AddComponent(projEntity, transform);
	world.AddComponent(projEntity, collider);
	world.AddComponent(projEntity, projectile.velocity);
	world.AddComponent(projEntity, projectile.texture);
	world.AddComponent(projEntity, projectile.shader);
	world.AddComponent(projEntity, projectile.damage);
	world.AddComponent(projEntity, LayerComponent({ "projectile" }));

	return projEntity;
}
