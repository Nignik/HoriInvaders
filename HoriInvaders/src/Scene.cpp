#include "Scene.h"

#include <fstream>

Scene::Scene(fs::path yamlPath, std::string sceneName)
{
	/*
	try
	{
		auto scenes = YAML::LoadFile(yamlPath.string());
		auto scene = scenes[sceneName];

		auto& world = Hori::Ecs::GetInstance();

		m_spawners.reserve(scene["enemy_spawners"].size())
		for (auto spawner : scene["enemy_spawners"])
		{
			Hori::Transform transform{ {spawner["position"][0].as<float>(), spawner["position"][0].as<float>()}, 0.0f, {1.0f, 1.0f} };
			float cooldown = spawner["cooldown"].as<float>();
			std::string enemy = spawner["enemy"].as<std::string>();

			Entity spawnerEntity = world.CreateEntity();
			world.AddComponents(spawnerEntity, SpawnerComponent(), transform, CooldownComponent{})
			m_spawners.push_back(spawnerEntity)
		}
	}
	*/
};
