#include "DeathSystem.h"

#include <Core/EventManager.h>
#include <Core/Ecs.h>
#include <iostream>

#include "Components.h"
#include "Events.h"

DeathSystem::DeathSystem()
{

}

// TODO: Make it work with new Spawner
void DeathSystem::Update(float deltaTime)
{
	auto& world = Hori::Ecs::GetInstance();

	for (auto entity : world.GetEntitiesWith<Health>())
	{
		auto health = world.GetComponent<Health>(entity);
		if (health->value > 0)
			continue;
		
		if (world.HasComponents<EnemyComponent>(entity))
		{
			Hori::EventManager::GetInstance().AddEvents<EnemyDeathEvent>(EnemyDeathEvent{ entity });
			std::cout << "Log: Enemy died\n";
		}
		world.RemoveEntity(entity);
	}
}
