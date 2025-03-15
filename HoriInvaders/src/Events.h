#pragma once

struct EnemyDeathEvent : public Hori::IEvent
{
	EnemyDeathEvent(Hori::Entity deadEnemy)
		: deadEnemy(deadEnemy)
	{
	}

	Hori::Entity deadEnemy{};
};
