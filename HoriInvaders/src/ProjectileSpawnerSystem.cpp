#include "ProjectileSpawnerSystem.h"
#include "Gun.h"
#include "ProjectileFactoryComponent.h"
#include "Enemy.h"
#include "Player.h"
#include "CooldownComponent.h"

#include <World.h>
#include <Core/Sprite.h>
#include <Core/Transform.h>
#include <Core/Renderer.h>
#include <Core/Collider.h>
#include <cmath>

ProjectileSpawnerSystem::ProjectileSpawnerSystem()
{

}

void ProjectileSpawnerSystem::Update(float deltaTime)
{
	auto& world = Hori::World::GetInstance();

	// For every entity that has a gun component shoot all projectiles
	for (auto shooterEntity : world.GetEntitiesWithComponents<GunComponent>())
	{
		auto gun = world.GetComponent<GunComponent>(shooterEntity);
		auto factory = world.GetComponent<ProjectileFactoryComponent>(shooterEntity);
		assert(factory != nullptr && "Entity has a gun but doesn't have projectile factory component: not allowed");

		for (auto& projectilePrototype : gun->projectilePrototypes)
		{
			auto cooldownComponent = world.GetComponent<CooldownComponent>(gun->reaload[projectilePrototype]);
			if (!cooldownComponent->ready)
				continue;
			cooldownComponent->ready = false;
			
			auto projectile = Spawn(projectilePrototype, shooterEntity);
			factory->projectileEntities.insert(projectile);
		}

		// Check for every projectile, whether it has left the screen, if so delete
		std::vector<Hori::Entity> removed;
		for (auto& projEntity : factory->projectileEntities)
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
			factory->projectileEntities.erase(projEntity);
		}
	}
}

// Creates entity in the world
Hori::Entity ProjectileSpawnerSystem::Spawn(Hori::Entity& projectilePrototype, Hori::Entity& shooterEntity)
{
	auto& world = Hori::World::GetInstance();

	auto shooterTransform = world.GetComponent<Hori::Transform>(shooterEntity);
	assert(shooterTransform != nullptr && "Entity with a gun doesn't have a transform");

	auto projectile = world.Clone(projectilePrototype);
	world.GetComponent<Hori::Transform>(projectile)->position = shooterTransform->position;
	world.GetComponent<Hori::SphereCollider>(projectile)->transform = Hori::SphereCollider(*shooterTransform, true).transform;

	if (world.HasComponents<EnemyComponent>(shooterEntity))
		world.AddComponents(projectile, EnemyProjectileComponent());
	else if (world.HasComponents<PlayerComponent>(shooterEntity))
		world.AddComponents(projectile, PlayerProjectileComponent());

	return projectile;
}
