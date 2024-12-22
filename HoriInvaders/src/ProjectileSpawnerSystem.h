#pragma once

#include <Core/System.h>
#include <Entity.h>

#include "ProjectileBlueprint.h"

/*
	Class responsible for instantiating projectiles in the world
*/
class ProjectileSpawnerSystem : public Hori::System
{
public:
	ProjectileSpawnerSystem();

	void Update(float deltaTime) override;

private:
	Hori::Entity Spawn(ProjectileBlueprint& projectile);

};