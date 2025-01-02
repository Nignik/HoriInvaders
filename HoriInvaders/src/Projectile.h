#pragma once

#include <filesystem>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <Core/Texture.h>
#include <Core/ResourceManager.h>
#include <Core/VelocityComponent.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <World.h>
#include <Core/Collider.h>
#include <Core/Sprite.h>

#include "DamageComponent.h"
#include "CooldownComponent.h"

namespace fs = std::filesystem;

// Not sure if this is the best way, but it's simple
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
	auto cooldown = CooldownComponent(projectileData["cooldown"].as<float>());
	auto speed = projectileData["speed"].as<float>();
	auto dir = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(projectileData["direction"].as<float>()));
	auto velocity = Hori::VelocityComponent(dir, speed);
	Hori::Transform transform = {
		.position = glm::vec3(),
		.rotation = glm::degrees(std::atan2(dir.x, dir.y)),
		.scale = { 25.0f, 25.0f }
	};

	auto& world = Hori::World::GetInstance();
	Hori::Entity projectilePrototype = world.CreateEntity();
	world.AddComponents(projectilePrototype, texture, shader, damage, cooldown, velocity, transform, Hori::SphereCollider(transform, true), Hori::Sprite());

	return projectilePrototype;
}

