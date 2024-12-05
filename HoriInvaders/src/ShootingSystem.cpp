#include "ShootingSystem.h"
#include "Gun.h"
#include "ProjectileFactory.h"

#include <World.h>
#include <Core/Sprite.h>
#include <Core/Transform.h>

ShootingSystem::ShootingSystem()
{

}

void ShootingSystem::Update(float deltaTime)
{
	auto& world = Hori::World::GetInstance();
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
			
			factory.projectileEntities.push_back(projEntity);
		}

		// Only for test, needs to be implemented fully
		// It fucking works but really need to make projectileEntities vector into a set
		std::vector<int> removed;
		for (int i = 0; i < factory.projectileEntities.size(); i++)
		{
			auto& projEntity = factory.projectileEntities[i];
			auto& position = world.GetComponent<Hori::Transform>(projEntity).position;
			if (position.x > 400.0f || position.y > 400.0f)
			{
				world.RemoveEntity(projEntity);
				removed.push_back(i);
			}
		}

		int cnt = 0;
		for (auto idx : removed)
		{
			auto& arr = factory.projectileEntities;
			arr.erase(arr.begin() + idx - cnt);
			cnt++;
		}
	}
}

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
