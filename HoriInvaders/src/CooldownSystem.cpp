#include "CooldownSystem.h"
#include "CooldownComponent.h"

#include <World.h>

void CooldownSystem::Update(float dt)
{
	auto& world = Hori::World::GetInstance();

	for (auto entity : world.GetEntitiesWithComponents<CooldownComponent>())
	{
		auto cd = world.GetComponent<CooldownComponent>(entity);

		if (cd->ready)
			continue;

		cd->currentCooldown -= dt;
		if (cd->currentCooldown <= 0)
		{
			cd->ready = true;
			cd->currentCooldown = cd->totalCooldown;
		}
	}
}
