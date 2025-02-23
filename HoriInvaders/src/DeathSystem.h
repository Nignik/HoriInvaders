#pragma once

#include <System.h>
#include <Core/Ecs.h>

class DeathSystem : public Hori::System
{
public:
	DeathSystem();
	void Update(float deltaTime) override;

private:

};