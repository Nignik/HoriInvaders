#include "ShootingSystem.h"
#include "Gun.h"

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
		for (int i = 0; i < gun.projectiles.size(); i++)
		{
			auto& projectile = gun.projectiles[i];
			gun.reload[i] -= deltaTime;
			if (gun.reload[i] > 0)
				continue;

			const auto& projEntity = world.CreateEntity();
			world.AddComponent(projEntity, Hori::Sprite());
			world.AddComponent(projEntity, Hori::Transform({ 300.0f, 300.0f }, 0.0f, { 25.0f, 25.0f }));
			world.AddComponent(projEntity, projectile.velocity);
			world.AddComponent(projEntity, projectile.texture);
			world.AddComponent(projEntity, projectile.shader);

			gun.reload[i] = projectile.cooldown;
		}
	}
}
