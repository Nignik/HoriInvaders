#include "CooldownSystem.h"

#include <Core/Ecs.h>

#include "Components.h"

void CooldownSystem::Update(float dt)
{
	auto& world = Hori::Ecs::GetInstance();

	for (auto entity : world.GetEntitiesWith<CooldownComponent>())
	{
		auto cds = world.GetComponent<CooldownComponent>(entity);

		for (auto& cd : cds->cooldowns)
		{
			if (cd.ready)
				continue;

			cd.current -= dt;
			if (cd.current <= 0)
			{
				cd.ready = true;
				cd.current = cd.total;
			}
		}
	}
}
