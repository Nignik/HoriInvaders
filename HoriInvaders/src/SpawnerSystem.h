#pragma once

#include <System.h>
#include <Core/Components.h>
#include <Entity.h>

#include "Entities.h"

/*
	Class responsible for instantiating projectiles in the world
*/
class SpawnerSystem : public Hori::System
{
public:
	SpawnerSystem();

	void Update(float deltaTime) override;

private:
	Hori::Entity SpawnProjectile(Hori::Entity& prototype, Hori::Entity& spawnerEntity);

};