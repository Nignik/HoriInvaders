#include "CooldownSystem.h"

#include <Core/Ecs.h>

#include "Components.h"

void CooldownSystem::Update(float dt)
{
	auto& world = Hori::Ecs::GetInstance();

	for (auto entity : world.GetEntitiesWith<Cooldown>())
	{
		auto cds = world.GetComponent<Cooldown>(entity);

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
