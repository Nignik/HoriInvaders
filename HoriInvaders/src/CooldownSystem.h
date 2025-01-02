#pragma once

#include <Core/System.h>

class CooldownSystem : public Hori::System
{
public:
	void Update(float dt) override;
};