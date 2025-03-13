#pragma once

#include <filesystem>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <Core/ResourceManager.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <Core/Ecs.h>
#include <Core/Collider.h>
#include <Core/PrimitivesGeneration.h>
#include <Core/Components.h>

#include "DamageComponent.h"
#include "CooldownComponent.h"
#include "HealthComponent.h"

namespace fs = std::filesystem;

struct EnemyProjectileComponent
{

};

struct PlayerProjectileComponent
{

};

inline Hori::Entity createProjectilePrototype(YAML::Node projectileData)
{
	auto& world = Hori::Ecs::GetInstance();
	auto& resourceMng = Hori::ResourceManager::GetInstance();

	fs::path texturePath = projectileData["sprite"].as<std::string>();
	fs::path shaderPath = projectileData["shader"].as<std::string>();

	auto spriteHandle = resourceMng.Load<Hori::SpriteComponent>(texturePath);
	auto shaderHandle = resourceMng.Load<Hori::ShaderComponent>(shaderPath);

	auto sprite = *resourceMng.Get(spriteHandle);
	auto shader = *resourceMng.Get(shaderHandle);

	auto damage = DamageComponent(projectileData["damage"].as<float>());
	auto health = HealthComponent(1000000);

	auto speed = projectileData["speed"].as<float>();
	auto dir = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(projectileData["direction"].as<float>() - 90.f));
	auto velocity = Hori::VelocityComponent(dir, speed);
	auto scale = projectileData["size"].as<float>();

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

