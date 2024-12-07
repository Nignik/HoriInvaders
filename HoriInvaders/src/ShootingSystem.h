#pragma once

#include <Core/System.h>
#include <Entity.h>

#include "Projectile.h"

/*
	Class responsible for creating entities from projectiles descriptions
*/
class ShootingSystem : public Hori::System
{
public:
	ShootingSystem();

	void Update(float deltaTime) override;

private:
	Hori::Entity Shoot(Projectile& projectile);

};