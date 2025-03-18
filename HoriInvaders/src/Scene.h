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
	bool Init();
	void InitSystems();

	bool Reload();
private:
	Hori::ResourceHandle<YAML::Node> m_handle{};
	std::vector<Hori::Entity> m_entities{};


};
