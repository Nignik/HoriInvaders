#include "DamageSystem.h"
#include "HealthComponent.h"
#include "DamageComponent.h"
#include "EnemyFactoryComponent.h"
#include "Player.h"

#include <Core/EventManager.h>
#include <World.h>
#include <Core/Collider.h>
#include <iostream>

void DamageSystem::Update(float deltaTime)
{
	auto& eventMng = Hori::EventManager::GetInstance();
	auto& world = Hori::World::GetInstance();

	// TODO: instead of popping the event, get all events
	// Process every collision to check if any entities should take damage
	auto event = eventMng.PopEvent<Hori::TriggerEvent>();
	while (event != nullptr)
	{
		Hori::Entity entityA = event->entityA;
		Hori::Entity entityB = event->entityB;
		for (int i = 0; i < 2; i++)
		{
			auto health = world.GetComponent<HealthComponent>(entityA);
			auto damage = world.GetComponent<DamageComponent>(entityB);
			
			if (health == nullptr || damage == nullptr)
			{
				std::swap(entityA, entityB);
				continue;
			}

			if ((world.HasComponents<PlayerComponent>(entityA) && world.HasComponents<EnemyProjectileComponent>(entityB)) ||
				(world.HasComponents<EnemyComponent>(entityA) && world.HasComponents<PlayerProjectileComponent>(entityB)))
			{
				health->value -= (int)damage->value;
				std::cout << health->value << std::endl;

				if (health->value <= 0 && world.HasComponents<EnemyComponent>(entityA))
				{
					eventMng.AddEvents<EnemyDeathEvent>(entityA);
				}
			}

			// Swap the entities so that next loop checks them in reverse
			std::swap(entityA, entityB);
		}

		event = eventMng.PopEvent<Hori::TriggerEvent>();
	}
}
