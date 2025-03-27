#include <iostream>

#include <HoriEngine.h>
#include <Core/DebugUIComponents.h>
#include <Core/Components.h>
#include <Core/YamlInspector.h>
#include <glm/glm.hpp>

#include "Components.h"
#include "Entities.h"
#include "Events.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "ComponentParser.h"

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

	world.AddSystem<SceneSystem>(SceneSystem{});

	auto yamlInspector = world.CreateEntity();
	Hori::YamlInspector yamlComp{"C:/DEV/GamesDev/HoriInvaders/HoriInvaders"};
	yamlComp.Open("data/test_scene/enemies/enemy_1.yaml");
	world.AddComponents(yamlInspector, std::move(yamlComp));

	auto fileBrowser = world.CreateEntity();
	world.AddComponents(fileBrowser, Hori::FileBrowserComponent("file browser", "./"));
	
	auto sceneHandle = resourceMng.Load<YAML::Node>("data/test_scene/scene.yaml");
	world.AddSingletonComponent(Scene{ sceneHandle });
	Scene* scene = world.GetSingletonComponent<Scene>();
	scene->Init();
	scene->InitSystems();
	
	Hori::Entity sceneReloadButton = world.CreateEntity();
	Hori::ButtonComponent reloadScene("reload scene", std::bind(&Scene::Reload, scene));
	world.AddComponents(sceneReloadButton, std::move(reloadScene));
	
	Hori::Entity openYamlButton = world.CreateEntity();
	Hori::ButtonComponent openYaml("open yaml", [&world, yamlInspector, fileBrowser]() {
		auto yamlComponent = world.GetComponent<Hori::YamlInspector>(yamlInspector);
		auto fileBrowserComponent = world.GetComponent<Hori::FileBrowserComponent>(fileBrowser);
		if (fileBrowserComponent->selectedFilePath.extension() != ".yaml")
		{
			std::cout << "Tried to open " << fileBrowserComponent->selectedFilePath << "that is not yaml as yaml\n";
			return;
		}
		yamlComponent->Open(fileBrowserComponent->selectedFilePath);
	});
	world.AddComponents(openYamlButton, std::move(openYaml));

	Hori::Entity saveYamlEntity = world.CreateEntity();
	Hori::ButtonComponent saveYamlButton{ "save yaml", std::bind(&Hori::YamlInspector::Save, world.GetComponent<Hori::YamlInspector>(yamlInspector)) };
	world.AddComponents(saveYamlEntity, std::move(saveYamlButton));

	engine.Run();

	return 0;
}
