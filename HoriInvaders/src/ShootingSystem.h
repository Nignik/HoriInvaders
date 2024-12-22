#pragma once

#include <Core/System.h>
#include <Entity.h>

#include "Projectile.h"

/*
	Class responsible for instantiating projectiles in the world
*/
class ShootingSystem : public Hori::System
{
public:
	ShootingSystem();

	void Update(float deltaTime) override;

private:
	Hori::Entity Shoot(ProjectileBlueprint& projectile);

};