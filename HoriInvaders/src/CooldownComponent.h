#pragma once


/*
* Component used to track the cooldown.
* Ready has to be set to false manually, but sets to true automatically
*/
struct CooldownComponent
{
	CooldownComponent(float totalCooldown, bool ready = true)
		: totalCooldown(totalCooldown),
		currentCooldown(totalCooldown),
		ready(ready)
	{
	}

	float totalCooldown = 0.f;
	float currentCooldown = 0.f;
	bool ready = true;
};