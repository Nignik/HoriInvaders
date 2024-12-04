#include "ShootingSystem.h"
#include "Gun.h"

#include <World.h>

ShootingSystem::ShootingSystem()
{

}

void ShootingSystem::Update(float deltaTime)
{
	auto& world = Hori::World::GetInstance();
	for (const auto& entity : world.GetEntitiesWithComponents<Gun>())
	{
		auto& gun = world.GetComponent<Gun>(entity);
		for (auto& projectile : gun.projectiles)
		{
			/*const auto& projEntity = world.CreateEntity();
			world.AddComponent(projEntity, projectile.velocity);
			world.AddComponent(projEntity, projectile.texture);*/
			std::cout << "shooting projectile" << std::endl;
		}
	}
}
