#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <Core/Ecs.h>
#include <Core/Collider.h>
#include <Core/PrimitivesGeneration.h>
#include <Core/Components.h>
#include <Core/ResourceManager.h>
#include <yaml-cpp/yaml.h>

#include "Components.h"

inline Hori::Entity createProjectilePrototype(YAML::Node data)
{
	auto& world = Hori::Ecs::GetInstance();
	auto& resourceMng = Hori::ResourceManager::GetInstance();

	fs::path texturePath = data["sprite"].as<std::string>();
	fs::path shaderPath = data["shader"].as<std::string>();

	auto spriteHandle = resourceMng.Load<Hori::SpriteComponent>(texturePath);
	auto shaderHandle = resourceMng.Load<Hori::ShaderComponent>(shaderPath);

	auto sprite = *resourceMng.Get(spriteHandle);
	auto shader = *resourceMng.Get(shaderHandle);

	auto damage = DamageComponent(data["damage"].as<float>());
	auto health = HealthComponent(1000000);

	auto speed = data["speed"].as<float>();
	auto dir = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(data["direction"].as<float>() - 90.f));
	auto velocity = Hori::VelocityComponent(dir, speed);

	auto scale = data["size"].as<float>();
	Hori::TransformComponent transform = {
		.position = glm::vec3(),
		.rotation = glm::degrees(std::atan2(dir.x, dir.y)),
		.scale = { scale, scale }
	};

	auto vertices = generateCircleVertices(0.5f, 10);
	auto wireframe = Hori::WireframeComponent(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

	Hori::Entity projectilePrototype = world.CreatePrototypeEntity();
	world.AddComponents(projectilePrototype, std::move(sprite), std::move(shader), std::move(damage), std::move(velocity), std::move(health), std::move(transform), Hori::SphereCollider(transform, true), Hori::SpriteComponent(), std::move(wireframe));

	return projectilePrototype;
}
