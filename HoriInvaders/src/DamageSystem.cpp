#include "DamageSystem.h"
#include "HealthComponent.h"
#include "LayerComponent.h"
#include "DamageComponent.h"

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
			auto layerA = world.GetComponent<LayerComponent>(entityA);
			auto damage = world.GetComponent<DamageComponent>(entityB);
			auto layerB = world.GetComponent<LayerComponent>(entityB);
			
			// Swap the entities so that next loop checks them in reverse
			std::swap(entityA, entityB);

			if (health == nullptr || layerA == nullptr || damage == nullptr || layerB == nullptr)
				continue;

			if ((layerA->Contains({ "player" }) && layerB->Contains({ "projectile", "enemy" })) ||
				(layerA->Contains({ "enemy" }) && layerB->Contains({ "projectile", "player" })))
			{
				health->value -= (int)damage->value;
				std::cout << health->value << std::endl;
			}
		}

		event = eventMng.PopEvent<Hori::TriggerEvent>();
	}
}
