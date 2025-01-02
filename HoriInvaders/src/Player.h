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
#include <Core/Renderer.h>

#include "HealthComponent.h"
#include "ProjectileFactoryComponent.h"

namespace fs = std::filesystem;

struct PlayerComponent
{

};

struct Player
{
public:
	Player(Hori::Transform transform, float speed, int health)
		: entity(Hori::World::GetInstance().CreateEntity())
	{
		auto& world = Hori::World::GetInstance();

		world.AddComponents(entity, transform);
		world.AddComponents(entity, Hori::VelocityComponent({ 0.0f, 0.0f }, speed));
		world.AddComponents(entity, HealthComponent(health));
		world.AddComponents(entity, Hori::Sprite());
		world.AddComponents(entity, Hori::Controller());
		world.AddComponents(entity, Hori::SphereCollider(transform));
		world.AddComponents(entity, Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs"));
		world.AddComponents(entity, Hori::LoadTextureFromFile("resources/textures/awesomeface.png", true));
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

		world.AddComponents(entity, PlayerComponent());
		world.AddComponents(entity, transform);
		world.AddComponents(entity, Hori::VelocityComponent({ 0.0f, 0.0f }, speed));
		world.AddComponents(entity, HealthComponent(health));
		world.AddComponents(entity, Hori::LoadShaderFromFile(shaderPath.replace_extension(".vs"), shaderPath.replace_extension(".fs")));
		world.AddComponents(entity, Hori::LoadTextureFromFile(spritePath, true));
		world.AddComponents(entity, Hori::Sprite());
		world.AddComponents(entity, Hori::Controller());
		world.AddComponents(entity, Hori::SphereCollider(transform));
		world.AddComponents(entity, ProjectileFactoryComponent());
	}


	Hori::Entity entity;
};