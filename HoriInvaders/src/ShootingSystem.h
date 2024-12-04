#pragma once

#include <Core/System.h>

class ShootingSystem : public Hori::System
{
public:
	ShootingSystem();

	void Update(float deltaTime) override;

};