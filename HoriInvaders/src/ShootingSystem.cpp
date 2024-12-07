#include "ShootingSystem.h"
#include "Gun.h"
#include "ProjectileFactory.h"

#include <World.h>
#include <Core/Sprite.h>
#include <Core/Transform.h>
#include <Core/Renderer.h>

ShootingSystem::ShootingSystem()
{

}

void ShootingSystem::Update(float deltaTime)
{
	auto& world = Hori::World::GetInstance();

	// For every entity that has a gun component shoot all projectiles
	for (const auto& entity : world.GetEntitiesWithComponents<Gun>())
	{
		auto& gun = world.GetComponent<Gun>(entity);
		auto& factory = world.GetComponent<ProjectileFactory>(entity);
		for (int i = 0; i < gun.projectiles.size(); i++)
		{
			auto& projectile = gun.projectiles[i];
			gun.reload[i] -= deltaTime;
			if (gun.reload[i] > 0)
				continue;
			
			auto projEntity = Shoot(projectile);
			gun.reload[i] = projectile.cooldown;
			
			factory.projectileEntities.insert(projEntity);
		}

		// Check for every projectile, whether it has left the screen, if so delete
		std::vector<Hori::Entity> removed;
		for (auto& projEntity : factory.projectileEntities)
		{
			auto& position = world.GetComponent<Hori::Transform>(projEntity).position;
			const auto& screenSize = Hori::Renderer::GetInstance().GetScreenSize();
			if (position.x < 0 || position.x > screenSize.x || position.y < 0 || position.y > screenSize.y)
			{
				world.RemoveEntity(projEntity);
				removed.push_back(projEntity);
			}
		}

		for (auto& projEntity : removed)
		{
			factory.projectileEntities.erase(projEntity);
		}
	}
}

// Creates entity in the world
Hori::Entity ShootingSystem::Shoot(Projectile& projectile)
{
	auto& world = Hori::World::GetInstance();

	const auto& projEntity = world.CreateEntity();
	world.AddComponent(projEntity, Hori::Sprite());
	world.AddComponent(projEntity, Hori::Transform({ 300.0f, 300.0f }, 0.0f, { 25.0f, 25.0f }));
	world.AddComponent(projEntity, projectile.velocity);
	world.AddComponent(projEntity, projectile.texture);
	world.AddComponent(projEntity, projectile.shader);

	return projEntity;
}
