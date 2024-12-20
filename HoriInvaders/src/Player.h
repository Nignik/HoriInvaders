#pragma once

#include <World.h>
#include <Entity.h>
#include <Core/Transform.h>
#include <Core/Sprite.h>
#include <Core/VelocityComponent.h>
#include <Core/Controller.h>
#include <Core/Collider.h>
#include <Core/ResourceManager.h>

#include "HealthComponent.h"

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

		world.AddComponent(entity, Hori::Sprite());
		world.AddComponent(entity, Hori::Controller());
		world.AddComponent(entity, Hori::SphereCollider(transform));
		world.AddComponent(entity, Hori::LoadShaderFromFile("shaders/sprite.vs", "shaders/sprite.fs"));
		world.AddComponent(entity, Hori::LoadTextureFromFile("resources/textures/awesomeface.png", true));
	}


	Hori::Entity entity;
};