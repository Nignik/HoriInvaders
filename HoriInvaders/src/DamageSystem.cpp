#include "DamageSystem.h"

#include <Core/EventManager.h>
#include <Core/Ecs.h>
#include <Core/Collider.h>
#include <Core/Renderer.h>
#include <iostream>

#include "Components.h"

void DamageSystem::Update(float dt)
{
	ProcessTriggers();
	ProcessBounds();
}

void DamageSystem::ProcessTriggers()
{
	auto& eventMng = Hori::EventManager::GetInstance();
	auto& world = Hori::Ecs::GetInstance();

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

			std::cout << (world.HasComponents<PlayerComponent>(entityA) && world.HasComponents<EnemyProjectileComponent>(entityB)) << '\n';
			std::cout << (world.HasComponents<EnemyComponent>(entityA) && world.HasComponents<PlayerProjectileComponent>(entityB)) << '\n';

			if ((world.HasComponents<PlayerComponent>(entityA) && world.HasComponents<EnemyProjectileComponent>(entityB)) ||
				(world.HasComponents<EnemyComponent>(entityA) && world.HasComponents<PlayerProjectileComponent>(entityB)))
			{
				health->value -= (int)damage->value;
				std::cout << health->value << std::endl;
			}

			// Swap the entities so that next loop checks them in reverse
			std::swap(entityA, entityB);
		}

		event = eventMng.PopEvent<Hori::TriggerEvent>();
	}
}

void DamageSystem::ProcessBounds()
{
	auto& world = Hori::Ecs::GetInstance();
	auto& eventMng = Hori::EventManager::GetInstance();
	auto cameraSize = Hori::Renderer::GetInstance().GetCameraSize();

	for (auto& entity : world.GetEntitiesWith<HealthComponent>())
	{
		auto transform = world.GetComponent<Hori::TransformComponent>(entity);
		if (transform == nullptr)
			continue;

		auto pos = transform->position;
		if (pos.x < -cameraSize.x || pos.x > cameraSize.x || pos.y < -cameraSize.y || pos.y > cameraSize.y)
			world.GetComponent<HealthComponent>(entity)->value = 0;
	}
}
