#include "SceneSystem.h"

#include "Scene.h"

void SceneSystem::Update(float dt)
{
	auto& world = Hori::Ecs::GetInstance();
	
	Scene* scene = world.GetSingletonComponent<Scene>();
	if (scene && scene->IsComplete())
	{
		*scene = Scene{ scene->GetNextSceneHandle() };
		scene->Init();
	}
}
