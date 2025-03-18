#include <iostream>

#include <HoriEngine.h>
#include <Core/Collider.h>
#include <Core/DebugUIComponents.h>
#include <Core/YamlInspectorComponent.h>
#include <Core/Components.h>
#include <glm/glm.hpp>

#include "Components.h"
#include "Entities.h"
#include "EntityPrototypes.h"
#include "Events.h"
#include "Scene.h"

using namespace std;

int main()
{
	auto& engine = Hori::Engine::GetInstance();
	engine.InitSystems();
	engine.InitSingletonComponents();
	engine.InitDebugSystems();

	auto& renderer = Hori::Renderer::GetInstance();
	auto& world = Hori::Ecs::GetInstance();
	auto& resourceMng = Hori::ResourceManager::GetInstance();

	auto yamlInspector = world.CreateEntity();
	Hori::YamlInspectorComponent yamlComp;
	yamlComp.Open("data/test_scene/enemies/enemy_1.yaml");
	world.AddComponents(yamlInspector, std::move(yamlComp));

	auto fileBrowser = world.CreateEntity();
	world.AddComponents(fileBrowser, Hori::FileBrowserComponent("file browser", "C:/"));
	
	auto sceneHandle = resourceMng.Load<YAML::Node>("data/test_scene/scene.yaml");
	Scene scene(sceneHandle);
	scene.Init();
	scene.InitSystems();
	
	Hori::Entity sceneReloadButton = world.CreateEntity();
	Hori::ButtonComponent reloadScene("reload scene", std::bind(&Scene::Reload, &scene));
	world.AddComponents(sceneReloadButton, std::move(reloadScene));
	
	Hori::Entity openYamlButton = world.CreateEntity();
	Hori::ButtonComponent openYaml("open yaml", [&world, yamlInspector, fileBrowser]() {
		auto yamlComponent = world.GetComponent<Hori::YamlInspectorComponent>(yamlInspector);
		auto fileBrowserComponent = world.GetComponent<Hori::FileBrowserComponent>(fileBrowser);
		yamlComponent->Open(fileBrowserComponent->selectedFilePath);
	});
	world.AddComponents(openYamlButton, std::move(openYaml));

	engine.Run();

	return 0;
}
