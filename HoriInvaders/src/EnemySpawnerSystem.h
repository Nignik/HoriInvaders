#pragma once

#pragma once

#include <Core/System.h>
#include <Entity.h>

#include "EnemyBlueprint.h"

/*
	Class responsible for instantiating enemies in the world
*/
class EnemySpawnerSystem : public Hori::System
{
public:
	EnemySpawnerSystem();

	void Update(float deltaTime) override;

private:
	Hori::Entity Spawn(std::shared_ptr<EnemyBlueprint> enemy);

};