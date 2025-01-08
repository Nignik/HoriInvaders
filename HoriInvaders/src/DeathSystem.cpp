#include "DeathSystem.h"

#include <Core/EventManager.h>
#include <World.h>
#include <Core/Collider.h>
#include <iostream>

DeathSystem::DeathSystem()
{

}
// TODO: Make it work with new Spawner

void DeathSystem::Update(float deltaTime)
{
	//auto& eventMng = Hori::EventManager::GetInstance();
	//auto& world = Hori::World::GetInstance();

	//// Inefficient implemenation
	//auto event = eventMng.PopEvent<EnemyDeathEvent>();
	//while (event != nullptr)
	//{
	//	for (auto& factoryEntity : world.GetEntitiesWithComponents<EnemyFactoryComponent>())
	//	{
	//		auto factoryComp = world.GetComponent<EnemyFactoryComponent>(factoryEntity);
	//		if (factoryComp->entities.contains(event->deadEnemy))
	//		{
	//			std::cout << "Enemy died" << std::endl;
	//			world.RemoveEntity(event->deadEnemy);
	//			factoryComp->entities.erase(event->deadEnemy);
	//			break;
	//		}
	//	}

	//	event = eventMng.PopEvent<EnemyDeathEvent>();
	//}
	
}
