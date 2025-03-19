#pragma once

#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <World.h>
#include <Core/HoriEngine.h>
#include <Core/ResourceManager.h>


class Scene
{
public:
	Scene(Hori::ResourceHandle<YAML::Node> handle);
	~Scene();
	Scene& operator=(const Scene&);

	bool Init();
	void InitSystems();

	bool Reload();
	bool IsComplete() const;
	Hori::ResourceHandle<YAML::Node> GetNextSceneHandle() const;
private:
	Hori::ResourceHandle<YAML::Node> m_handle{};
	std::vector<Hori::Entity> m_entities{};
	Hori::ResourceHandle<YAML::Node> m_nextSceneHandle{};
	int m_enemyCount = 0;

	void Clear();
};
