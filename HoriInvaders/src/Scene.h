#pragma once

#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <World.h>
#include <Core/HoriEngine.h>

namespace fs = std::filesystem;

class Scene
{
public:
	Scene(fs::path yamlPath, std::string sceneName);

private:
	std::vector<Hori::Entity> m_spawners{};
};
