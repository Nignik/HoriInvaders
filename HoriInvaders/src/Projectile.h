#pragma once

#include <filesystem>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <Core/Texture.h>
#include <Core/ResourceManager.h>
#include <Core/VelocityComponent.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <Core/Ecs.h>
#include <Core/Collider.h>
#include <Core/Sprite.h>
#include <Core/PrimitivesGeneration.h>
#include <Core/WireframeComponent.h>

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
	fs::path spritePath = projectileData["sprite"].as<std::string>();
	fs::path shader_name = projectileData["shader"].as<std::string>();
	auto texture = Hori::LoadTextureFromFile(spritePath, true);
	auto shader = Hori::LoadShaderFromFile(shader_name.replace_extension(".vs"), shader_name.replace_extension(".fs"));

	auto damage = DamageComponent(projectileData["damage"].as<float>());
	auto health = HealthComponent(1000000);

	auto speed = projectileData["speed"].as<float>();
	auto dir = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(projectileData["direction"].as<float>() - 90.f));
	auto velocity = Hori::VelocityComponent(dir, speed);
	auto scale = projectileData["size"].as<float>();

	Hori::Transform transform = {
		.position = glm::vec3(),
		.rotation = glm::degrees(std::atan2(dir.x, dir.y)),
		.scale = { scale, scale }
	};

	auto vertices = generateCircleVertices(0.5f, 10);
	auto wireframe = Hori::WireframeComponent(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

	auto& world = Hori::Ecs::GetInstance();
	Hori::Entity projectilePrototype = world.CreatePrototypeEntity();
	world.AddComponents(projectilePrototype, std::move(texture), std::move(shader), std::move(damage), std::move(velocity), std::move(health), std::move(transform), Hori::SphereCollider(transform, true), Hori::Sprite(), std::move(wireframe));

	return projectilePrototype;
}

