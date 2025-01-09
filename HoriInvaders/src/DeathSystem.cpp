#include "DeathSystem.h"
#include "HealthComponent.h"
#include "SpawnerComponent.h"
#include "Projectile.h"

#include <Core/EventManager.h>
#include <World.h>
#include <Core/Collider.h>
#include <iostream>

DeathSystem::DeathSystem()
{

}

// TODO: Make it work with new Spawner
void DeathSystem::Update(float deltaTime)
{
	auto& world = Hori::World::GetInstance();

	for (auto entity : world.GetEntitiesWithComponents<HealthComponent>())
	{
		auto health = world.GetComponent<HealthComponent>(entity);
		if (health->value > 0)
			continue;

		world.RemoveEntity(entity);
	}
}
