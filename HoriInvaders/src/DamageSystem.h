#pragma once

#include <Core/System.h>


class DamageSystem : public Hori::System
{
public:
	void Update(float dt) override;

private:
	// Damages the entities which entered a damaging trigger
	void ProcessTriggers();

	// Kill the entities which go too far out of camera bounds
	void ProcessBounds();

};