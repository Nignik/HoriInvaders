#pragma once

#include <Core/System.h>
#include <World.h>

class DeathSystem : public Hori::System
{
public:
	DeathSystem();
	void Update(float deltaTime) override;

private:

};