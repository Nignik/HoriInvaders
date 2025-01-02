#pragma once

#include <Core/System.h>
#include <Core/Transform.h>
#include <Entity.h>

#include "Projectile.h"

/*
	Class responsible for instantiating projectiles in the world
*/
class ProjectileSpawnerSystem : public Hori::System
{
public:
	ProjectileSpawnerSystem();

	void Update(float deltaTime) override;

private:
	Hori::Entity Spawn(Hori::Entity& projectilePrototype, Hori::Entity& shooterEntity);

};