#include "DamageSystem.h"
#include "HealthComponent.h"

#include <Core/EventManager.h>
#include <World.h>
#include <Core/Collider.h>
#include <iostream>

void DamageSystem::Update(float deltaTime)
{
	auto& eventMng = Hori::EventManager::GetInstance();
	auto& world = Hori::World::GetInstance();

	auto event = eventMng.PopEvent<Hori::TriggerEvent>();
	// Only for testing, code repetition, and constant damage to be changed.
	while (event != nullptr)
	{
		if (world.HasComponent<HealthComponent>(event->entityA.GetID()))
		{
			auto& health = world.GetComponent<HealthComponent>(event->entityA);
			health.value -= 10;
			std::cout << health.value << std::endl;
		}
		if (world.HasComponent<HealthComponent>(event->entityB.GetID()))
		{
			auto& health = world.GetComponent<HealthComponent>(event->entityB);
			health.value -= 10;
			std::cout << health.value << std::endl;
		}

		event = eventMng.PopEvent<Hori::TriggerEvent>();
	}
}
