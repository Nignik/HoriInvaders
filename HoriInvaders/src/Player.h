#pragma once

#include <filesystem>

#include <World.h>
#include <Entity.h>
#include <Core/Transform.h>
#include <Core/Sprite.h>
#include <Core/VelocityComponent.h>
#include <Core/Controller.h>
#include <Core/Collider.h>
#include <Core/ResourceManager.h>

#include "HealthComponent.h"
#include "LayerComponent.h"

namespace fs = std::filesystem;

struct Player
{
public:
	Player(Hori::Transform transform, float speed, int health)
		: entity(Hori::World::GetInstance().CreateEntity())
	{
		auto& world = Hori::World::GetInstance();

		world.AddComponent(entity, transform);
		world.AddComponent(entity, Hori::VelocityComponent({ 0.0f, 0.0f }, speed));
		world.AddComponent(entity, HealthComponent(health));
		world.AddComponent(entity, LayerComponent({"player"}));
		world.AddComponent(entity, Hori::Sprite());
		world.AddComponent(entity, Hori::Controller());
		world.AddComponent(entity, Hori::SphereCollider(transform));
		world.AddComponent(entity, Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs"));
		world.AddComponent(entity, Hori::LoadTextureFromFile("resources/textures/awesomeface.png", true));
	}

	Player(YAML::Node playerInfo)
		: entity(Hori::World::GetInstance().CreateEntity())
	{
		auto& world = Hori::World::GetInstance();

		auto screenDim = Hori::Renderer::GetInstance().GetScreenSize();
		Hori::Transform transform = {
			.position = {playerInfo["spawn"][0].as<float>() * screenDim.x, playerInfo["spawn"][1].as<float>() * screenDim.y},
			.rotation = 0.0f,
			.scale = {playerInfo["size"].as<float>(), playerInfo["size"].as<float>()}
		};
		fs::path shaderPath = playerInfo["shader"].as<std::string>();
		fs::path spritePath = playerInfo["sprite"].as<std::string>();
		float speed = playerInfo["speed"].as<float>();
		int health = playerInfo["health"].as<int>();

		world.AddComponent(entity, transform);
		world.AddComponent(entity, Hori::VelocityComponent({ 0.0f, 0.0f }, speed));
		world.AddComponent(entity, HealthComponent(health));
		world.AddComponent(entity, Hori::LoadShaderFromFile(shaderPath.replace_extension(".vs"), shaderPath.replace_extension(".fs")));
		world.AddComponent(entity, Hori::LoadTextureFromFile(spritePath, true));
		world.AddComponent(entity, LayerComponent({ "player" }));
		world.AddComponent(entity, Hori::Sprite());
		world.AddComponent(entity, Hori::Controller());
		world.AddComponent(entity, Hori::SphereCollider(transform));
	}


	Hori::Entity entity;
};